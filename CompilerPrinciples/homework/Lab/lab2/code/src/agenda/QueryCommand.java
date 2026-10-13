package agenda;

import java.util.List;

/** Command implementation that delegates to {@link AgendaManager#cmdQuery(List)}. */
public class QueryCommand implements Command {
    @Override
    public void execute(AgendaManager manager, List<String> tokens) {
        manager.cmdQuery(tokens);
    }
}
