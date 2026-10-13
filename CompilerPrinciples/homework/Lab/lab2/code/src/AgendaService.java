package agenda;

/** Entry point for the Agenda management application. */
public class AgendaService {
    /**
     * Main entry. Supports interactive and batch modes.
    * @param args program arguments; use "-batch fileName" to run batch mode
     */
    public static void main(String[] args) {
        AgendaManager manager = AgendaManager.getInstance();
        if (args.length == 2 && "-batch".equals(args[0])) {
            manager.processBatch(args[1]);
            return;
        }
        manager.run();
    }
}
