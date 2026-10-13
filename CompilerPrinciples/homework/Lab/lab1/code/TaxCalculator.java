
/**
 * 个人所得税计算器类，支持超额累进税率计算。
 * <p>
 * 根据配置的税率表和起征点，计算应缴纳税额。
 * </p>
 * @author 林宏宇
 * @version 1.0
 */
public class TaxCalculator {

    /**
     * 税率和起征点配置。
     */
    private final TaxConfig config;

    /**
     * 构造计算器对象。
     * @param config 税率配置
     */
    public TaxCalculator(TaxConfig config) {
        this.config = config;
    }

    /**
     * 计算指定收入的应纳税额。
     * @param income 月收入
     * @return 应缴税额
     * @throws IllegalArgumentException 收入为负时抛出
     */
    public double computeTax(double income) {
        if (income < 0) {
            throw new IllegalArgumentException("Income must be non-negative.");
        }
        double taxable = income - config.getThreshold();
        if (taxable <= 0) {
            return 0.0;
        }

        double tax = 0.0;
        double previousUpper = 0.0;
        for (TaxBracket bracket : config.getBrackets()) {
            double upper = bracket.getUpperBound();
            double portion = Math.min(taxable, upper) - previousUpper;
            if (portion > 0) {
                tax += portion * bracket.getRate();
            }
            if (taxable <= upper) {
                break;
            }
            previousUpper = upper;
        }
        return tax;
    }
}
