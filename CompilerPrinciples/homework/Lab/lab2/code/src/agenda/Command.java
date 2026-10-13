package agenda;

import java.util.List;

/**
 * Command interface for the command pattern used by the application.
 */
public interface Command {
    /**
     * Execute this command.
     * @param manager the {@link AgendaManager} to operate on
     * @param tokens parsed command tokens (token[0] is command name)
     */
    void execute(AgendaManager manager, List<String> tokens);
}
