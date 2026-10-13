package agenda;

import java.util.List;

/** Command implementation that delegates to {@link AgendaManager#cmdAdd(List)}. */
public class AddCommand implements Command {
    @Override
    public void execute(AgendaManager manager, List<String> tokens) {
        manager.cmdAdd(tokens);
    }
}
