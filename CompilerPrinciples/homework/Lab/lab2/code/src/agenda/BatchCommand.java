package agenda;

import java.util.List;

/** Command implementation that executes a batch file via {@link AgendaManager#processBatch(String)}. */
public class BatchCommand implements Command {
    @Override
    public void execute(AgendaManager manager, List<String> tokens) {
        if (tokens.size() >= 2) manager.processBatch(tokens.get(1));
        else System.out.println("Usage: batch [fileName]");
    }
}
