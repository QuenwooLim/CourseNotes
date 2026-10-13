package agenda;

import java.util.List;

/**
 * Command implementation that delegates to {@link AgendaManager#cmdRegister(List)}.
 */
public class RegisterCommand implements Command {
    @Override
    public void execute(AgendaManager manager, List<String> tokens) {
        manager.cmdRegister(tokens);
    }
}
