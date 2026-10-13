import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;

/**
 * TaxCalculator单元测试类。
 */
public class TaxCalculatorTest {
    private TaxConfig config;
    private TaxCalculator calculator;

    @Before
    public void setUp() {
        config = new TaxConfig();
        calculator = new TaxCalculator(config);
    }

    @Test
    public void testZeroIncome() {
        assertEquals(0.0, calculator.computeTax(0), 0.001);
    }

    @Test
    public void testBelowThreshold() {
        assertEquals(0.0, calculator.computeTax(500), 0.001);
    }

    @Test
    public void testFirstBracket() {
        // 1000-800=200, 200*5% = 10
        assertEquals(10.0, calculator.computeTax(1000), 0.001);
    }

    @Test
    public void testMultiBracket() {
        // 4300-800=3500, 500*5%+1500*10%+1500*15%=25+150+225=400
        assertEquals(400.0, calculator.computeTax(4300), 0.001);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testNegativeIncome() {
        calculator.computeTax(-100);
    }

    @Test
    public void testSetThreshold() {
        config.setThreshold(3500);
        assertEquals(0.0, calculator.computeTax(3400), 0.001);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testInvalidThreshold() {
        config.setThreshold(-1);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testInvalidRates() {
        config.setRates(new double[]{0.05, 0.1});
    }
}
