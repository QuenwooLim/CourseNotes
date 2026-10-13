package agenda;

import java.util.*;

/** Represents a registered user and their meeting lists. */
public class User {
    private final String name;
    private final String password;
    private final List<Meeting> meetingsAll = new ArrayList<>();
    private final List<Meeting> meetingsCreated = new ArrayList<>();

    /**
     * Create a new user with given name and password.
     * @param name user name
     * @param password user's password
     */
    public User(String name, String password) {
        this.name = name; this.password = password;
    }

    /**
     * Return the user's name.
     * @return user's name
     */
    public String getName() { return name; }

    /**
     * Check whether the given password matches this user's password.
     * @param pwd the password to check
     * @return true if password matches
     */
    public boolean checkPassword(String pwd) { return password.equals(pwd); }

    /**
     * Add a meeting to this user's participation list.
     * @param m meeting to add
     */
    public void addMeeting(Meeting m) { meetingsAll.add(m); }

    /**
     * Record a meeting created by this user.
     * @param m meeting created by this user
     */
    public void addMeetingCreated(Meeting m) { meetingsCreated.add(m); }

    /**
     * Return all meetings this user participates in.
     * @return list of meetings this user participates in
     */
    public List<Meeting> getMeetings() { return meetingsAll; }

    /**
     * Return meetings created by this user.
     * @return list of meetings created by this user
     */
    public List<Meeting> getCreatedMeetings() { return meetingsCreated; }

    /**
     * Return true if this user has created a meeting with the given label.
     * @param label meeting label to check
     * @return true if a created meeting with the label exists
     */
    public boolean hasCreatedLabel(String label) { for (Meeting m : meetingsCreated) if (m.getLabel().equals(label)) return true; return false; }

    /**
     * Find a meeting created by this user identified by label.
     * @param label meeting label
     * @return the meeting if found, otherwise null
     */
    public Meeting findCreatedByLabel(String label) { for (Meeting m : meetingsCreated) if (m.getLabel().equals(label)) return m; return null; }

    /**
     * Remove a meeting from this user's participation list.
     * @param m meeting to remove
     */
    public void removeMeeting(Meeting m) { meetingsAll.remove(m); }

    /**
     * Remove a meeting from this user's created list.
     * @param m meeting to remove
     */
    public void removeCreated(Meeting m) { meetingsCreated.remove(m); }
}
