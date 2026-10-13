
/**
 * 税率档次类，表示超额累进税率的一个区间。
 * <p>
 * 每个对象包含该档的上限和税率。
 * </p>
 * @author 林宏宇
 * @version 1.0
 */
public class TaxBracket {
    /**
     * 本档税率的上限（含）。如为Double.POSITIVE_INFINITY表示无上限。
     */
    private final double upperBound;

    /**
     * 本档适用税率（如0.05表示5%）。
     */
    private final double rate;

    /**
     * 构造一个税率档对象。
     * @param upperBound 本档上限
     * @param rate 本档税率
     */
    public TaxBracket(double upperBound, double rate) {
        this.upperBound = upperBound;
        this.rate = rate;
    }

    /**
     * 获取本档上限。
     * @return 上限数值
     */
    public double getUpperBound() {
        return upperBound;
    }

    /**
     * 获取本档税率。
     * @return 税率
     */
    public double getRate() {
        return rate;
    }
}
