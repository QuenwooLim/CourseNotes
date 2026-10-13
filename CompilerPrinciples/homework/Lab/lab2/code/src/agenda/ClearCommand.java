package agenda;

import java.util.List;

/** Command implementation that delegates to {@link AgendaManager#cmdClear(List)}. */
public class ClearCommand implements Command {
    @Override
    public void execute(AgendaManager manager, List<String> tokens) {
        manager.cmdClear(tokens);
    }
}
