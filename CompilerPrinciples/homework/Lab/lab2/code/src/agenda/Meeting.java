package agenda;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

/** A meeting record between two registered users. */
public class Meeting {
    private final String label;
    private final LocalDateTime start;
    private final LocalDateTime end;
    private final String initiator;
    private final String other;
    private static final DateTimeFormatter FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd_HH:mm");
    /**
     * Construct a meeting with label, time range, initiator and other participant.
     * @param label meeting label (identifier)
     * @param start meeting start time
     * @param end meeting end time
     * @param initiator initiator (creator) user name
     * @param other the other participant user name
     */
    public Meeting(String label, LocalDateTime start, LocalDateTime end, String initiator, String other) {
        this.label = label; this.start = start; this.end = end; this.initiator = initiator; this.other = other;
    }

    /**
     * Return the meeting label (identifier).
     * @return meeting label
     */
    public String getLabel() { return label; }

    /**
     * Return meeting start time.
     * @return start time
     */
    public LocalDateTime getStart() { return start; }

    /**
     * Return meeting end time.
     * @return end time
     */
    public LocalDateTime getEnd() { return end; }

    /**
     * Return the initiator (creator) of the meeting.
     * @return initiator user name
     */
    public String getInitiator() { return initiator; }

    /**
     * Given a participant name, return the other participant's name.
     * @param name a participant name
     * @return the other participant's name
     */
    public String getOther(String name) { return initiator.equals(name) ? other : initiator; }

    /**
     * Return true if this meeting overlaps the given time interval.
     * @param s interval start
     * @param e interval end
     * @return true if the meeting overlaps [s, e)
     */
    public boolean overlaps(LocalDateTime s, LocalDateTime e) { return start.isBefore(e) && s.isBefore(end); }

    /**
     * Format meeting info for display to the given user.
     * @param user viewing user's name
     * @return formatted meeting line: start end label partner
     */
    public String formatForUser(String user) {
        String partner = getOther(user);
        return String.format("%s %s %s %s", start.format(FMT), end.format(FMT), label, partner);
    }
}
