package agenda;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 * JUnit5 tests for AgendaManager.
 */
public class AgendaManagerTest {

    @Test
    public void singletonInstanceIsSame() {
        AgendaManager m1 = AgendaManager.getInstance();
        AgendaManager m2 = AgendaManager.getInstance();
        assertSame(m1, m2, "getInstance should return the same singleton instance");
    }

    @Test
    public void registerCreatesUser() {
        AgendaManager m = AgendaManager.getInstance();
        String uname = "junitUser" + System.currentTimeMillis();
        m.processCommand("register " + uname + " pwd123");
        assertNotNull(m.getUser(uname), "User should be created after register command");
    }
}
