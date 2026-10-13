
/**
 * 税率配置类，包含起征点和所有税率档次。
 * <p>
 * 支持动态调整起征点和税率。
 * </p>
 * @author 林宏宇
 * @version 1.0
 */
public class TaxConfig {
    /**
     * 个人所得税起征点。
     */
    private double threshold;

    /**
     * 所有税率档次。
     */
    private TaxBracket[] brackets;

    /**
     * 构造默认税率配置。
     */
    public TaxConfig() {
        this.threshold = 1600.0;
        this.brackets = defaultBrackets();
    }

    /**
     * 获取当前起征点。
     * @return 起征点
     */
    public double getThreshold() {
        return threshold;
    }

    /**
     * 设置起征点。
     * @param threshold 新的起征点
     */
    public void setThreshold(double threshold) {
        if (threshold < 0) {
            throw new IllegalArgumentException("Threshold must be non-negative.");
        }
        this.threshold = threshold;
    }

    /**
     * 获取所有税率档次。
     * @return 税率档数组
     */
    public TaxBracket[] getBrackets() {
        return brackets;
    }


    /**
     * 设置所有税率和区间。
     * @param uppers 长度为5的上限数组，最后一档应为Double.POSITIVE_INFINITY
     * @param rates 长度为5的税率数组
     */
    public void setBrackets(double[] uppers, double[] rates) {
        if (uppers == null || rates == null || uppers.length != 5 || rates.length != 5) {
            throw new IllegalArgumentException("档位区间和税率数组长度必须为5。");
        }
        double last = 0;
        for (int i = 0; i < 4; i++) {
            if (uppers[i] <= last) throw new IllegalArgumentException("档位上限需递增且大于前一档");
            last = uppers[i];
        }
        if (uppers[4] != Double.POSITIVE_INFINITY) throw new IllegalArgumentException("最后一档上限必须为无穷大");
        this.brackets = new TaxBracket[] {
            new TaxBracket(uppers[0], rates[0]),
            new TaxBracket(uppers[1], rates[1]),
            new TaxBracket(uppers[2], rates[2]),
            new TaxBracket(uppers[3], rates[3]),
            new TaxBracket(uppers[4], rates[4])
        };
    }

    /**
    * 兼容原有，仅设置税率，区间不变。
    * @param rates 新的税率数组（长度需为5，单位为小数，如0.05表示5%）
     */
    public void setRates(double[] rates) {
        if (rates == null || rates.length != 5) {
            throw new IllegalArgumentException("Rates length must be 5.");
        }
        TaxBracket[] old = this.brackets;
        this.brackets = new TaxBracket[] {
            new TaxBracket(old[0].getUpperBound(), rates[0]),
            new TaxBracket(old[1].getUpperBound(), rates[1]),
            new TaxBracket(old[2].getUpperBound(), rates[2]),
            new TaxBracket(old[3].getUpperBound(), rates[3]),
            new TaxBracket(old[4].getUpperBound(), rates[4])
        };
    }

    /**
     * 获取默认税率档数组。
     * @return 默认税率档
     */
    public static TaxBracket[] defaultBrackets() {
        return new TaxBracket[] {
            new TaxBracket(500.0, 0.05),
            new TaxBracket(2000.0, 0.10),
            new TaxBracket(5000.0, 0.15),
            new TaxBracket(20000.0, 0.20),
            new TaxBracket(Double.POSITIVE_INFINITY, 0.25)
        };
    }
}
