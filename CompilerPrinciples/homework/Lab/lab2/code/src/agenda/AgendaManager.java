package agenda;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.*;
import java.io.*;

/** Manager that handles command parsing and maintains users and meetings in-memory. */
public class AgendaManager {
    private static final DateTimeFormatter FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd_HH:mm");
    // Eager singleton instance (饿汉式)
    private static final AgendaManager INSTANCE = new AgendaManager();
    private final Map<String, User> users = new HashMap<>();
    private final Scanner in;
    private final Map<String, Command> commands = new HashMap<>();

    // Private constructor for Singleton
    private AgendaManager() {
        in = new Scanner(System.in);
        // register commands
        commands.put("register", new RegisterCommand());
        commands.put("add", new AddCommand());
        commands.put("query", new QueryCommand());
        commands.put("delete", new DeleteCommand());
        commands.put("clear", new ClearCommand());
        commands.put("batch", new BatchCommand());
        commands.put("quit", new QuitCommand());
    }

    /**
     * Return the singleton instance of AgendaManager.
     * @return the singleton {@link AgendaManager} instance
     */
    public static AgendaManager getInstance() {
        return INSTANCE;
    }

    /** Run interactive loop reading commands from standard input. */
    public void run() {
        while (true) {
            System.out.print("$ ");
            String line;
            try {
                line = in.nextLine();
            } catch (NoSuchElementException e) {
                break;
            }
            if (line == null) break;
            line = line.trim();
            if (line.isEmpty()) continue;
            processCommand(line);
        }
    }

    /**
     * Parse and execute a single command line.
     * @param line the command line to parse and execute
     */
    public void processCommand(String line) {
        List<String> tokens = splitArgs(line);
        if (tokens.isEmpty()) return;
        String cmd = tokens.get(0).toLowerCase();
        Command command = commands.get(cmd);
        if (command != null) {
            command.execute(this, tokens);
        } else {
            System.out.println("Unknown command: " + cmd);
        }
    }

    private List<String> splitArgs(String line) {
        List<String> parts = new ArrayList<>();
        String[] p = line.split(" ");
        for (String s : p) if (!s.isEmpty()) parts.add(s);
        return parts;
    }

    /**
     * Handle the `register` command.
     * @param tokens command tokens: ["register", userName, password]
     */
    public void cmdRegister(List<String> tokens) {
        if (tokens.size() != 3) { System.out.println("Usage: register [userName] [password]"); return; }
        String name = tokens.get(1), pwd = tokens.get(2);
        if (users.containsKey(name)) { System.out.println("Error: user already exists"); return; }
        users.put(name, new User(name, pwd));
        System.out.println("Register success");
    }

    /**
     * Handle the `add` command to create a new meeting.
     * @param tokens command tokens: ["add", userName, password, other, start, end, title...]
     */
    public void cmdAdd(List<String> tokens) {
        if (tokens.size() < 7) { System.out.println("Usage: add [userName] [password] [other] [start] [end] [title]"); return; }
        String user = tokens.get(1), pwd = tokens.get(2), other = tokens.get(3);
        String startS = tokens.get(4), endS = tokens.get(5);
        String title = join(tokens, 6);
        User u1 = authenticate(user, pwd); if (u1 == null) return;
        User u2 = users.get(other); if (u2 == null) { System.out.println("Error: other user not found"); return; }
        LocalDateTime start, end;
        try { start = LocalDateTime.parse(startS, FMT); end = LocalDateTime.parse(endS, FMT); } catch (DateTimeParseException ex) { System.out.println("Error: bad time format, use yyyy-MM-dd_HH:mm"); return; }
        if (!start.isBefore(end)) { System.out.println("Error: start must be before end"); return; }
        if (u1.hasCreatedLabel(title)) { System.out.println("Error: meeting label already used"); return; }
        if (hasOverlap(u1, start, end) || hasOverlap(u2, start, end)) { System.out.println("Error: time overlap"); return; }
        Meeting m = new Meeting(title, start, end, user, other);
        u1.addMeetingCreated(m); u1.addMeeting(m); u2.addMeeting(m);
        System.out.println("Add success");
    }

    private String join(List<String> tokens, int from) {
        StringBuilder sb = new StringBuilder();
        for (int i = from; i < tokens.size(); i++) { if (i > from) sb.append(' '); sb.append(tokens.get(i)); }
        return sb.toString();
    }

    private boolean hasOverlap(User u, LocalDateTime s, LocalDateTime e) {
        for (Meeting m : u.getMeetings()) if (m.overlaps(s, e)) return true;
        return false;
    }

    /**
     * Handle the `query` command to list meetings within a time range.
     * @param tokens command tokens: ["query", userName, password, start, end]
     */
    public void cmdQuery(List<String> tokens) {
        if (tokens.size() != 5) { System.out.println("Usage: query [userName] [password] [start] [end]"); return; }
        String name = tokens.get(1), pwd = tokens.get(2), startS = tokens.get(3), endS = tokens.get(4);
        User u = authenticate(name, pwd); if (u == null) return;
        LocalDateTime qs, qe;
        try { qs = LocalDateTime.parse(startS, FMT); qe = LocalDateTime.parse(endS, FMT); } catch (DateTimeParseException ex) { System.out.println("Error: bad time format"); return; }
        List<Meeting> list = new ArrayList<>();
        for (Meeting m : u.getMeetings()) if (m.overlaps(qs, qe)) list.add(m);
        list.sort(Comparator.comparing(Meeting::getStart));
        for (Meeting m : list) System.out.println(m.formatForUser(name));
        System.out.println("Query done, " + list.size() + " meetings found");
    }

    /**
     * Handle the `delete` command to remove a meeting created by the user.
     * @param tokens command tokens: ["delete", userName, password, meetingId]
     */
    public void cmdDelete(List<String> tokens) {
        if (tokens.size() != 4) { System.out.println("Usage: delete [userName] [password] [meetingId]"); return; }
        String name = tokens.get(1), pwd = tokens.get(2), id = tokens.get(3);
        User u = authenticate(name, pwd); if (u == null) return;
        Meeting m = u.findCreatedByLabel(id);
        if (m == null) { System.out.println("Error: meeting not found or not creator"); return; }
        User other = users.get(m.getOther(name)); if (other != null) other.removeMeeting(m);
        u.removeMeeting(m); u.removeCreated(m);
        System.out.println("Delete success");
    }

    /**
     * Handle the `clear` command to delete all meetings created by the user.
     * @param tokens command tokens: ["clear", userName, password]
     */
    public void cmdClear(List<String> tokens) {
        if (tokens.size() != 3) { System.out.println("Usage: clear [userName] [password]"); return; }
        String name = tokens.get(1), pwd = tokens.get(2);
        User u = authenticate(name, pwd); if (u == null) return;
        List<Meeting> copy = new ArrayList<>(u.getCreatedMeetings());
        for (Meeting m : copy) {
            User other = users.get(m.getOther(name)); if (other != null) other.removeMeeting(m);
            u.removeMeeting(m); u.removeCreated(m);
        }
        System.out.println("Clear success");
    }

    /**
     * Execute commands from a batch file (one command per line).
     * @param fileName path to the batch file to process
     */
    public void processBatch(String fileName) {
        File f = new File(fileName);
        if (!f.exists()) { System.out.println("Error: batch file not found: " + fileName); return; }
        try (BufferedReader br = new BufferedReader(new FileReader(f))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim(); if (line.isEmpty() || line.startsWith("#")) continue;
                System.out.println("$ " + line);
                processCommand(line);
            }
        } catch (Exception ex) { System.out.println("Error reading batch: " + ex.getMessage()); }
    }

    private User authenticate(String name, String pwd) {
        User u = users.get(name);
        if (u == null) { System.out.println("Error: user not found"); return null; }
        if (!u.checkPassword(pwd)) { System.out.println("Error: authentication failed"); return null; }
        return u;
    }

    /**
     * Test helper: return registered user by name.
     * @param name the user name to look up
     * @return the {@link User} instance if registered, otherwise {@code null}
     */
    public User getUser(String name) { return users.get(name); }
}
