package agenda;

import java.util.List;

/** Command implementation that terminates the application. */
public class QuitCommand implements Command {
    @Override
    public void execute(AgendaManager manager, List<String> tokens) {
        System.out.println("Bye");
        System.exit(0);
    }
}
