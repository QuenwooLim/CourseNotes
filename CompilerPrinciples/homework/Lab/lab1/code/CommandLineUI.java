/**
 * 命令行用户交互界面类。
 * <p>
 * 负责菜单显示、用户输入、功能分发和异常处理。
 * </p>
 * @author 林宏宇
 * @version 1.0
 */
public class CommandLineUI {
    private final java.util.Scanner scanner;
    private final TaxConfig config;
    private final TaxCalculator calculator;

    /**
     * 构造命令行界面对象。
     * @param config 税率配置
     */
    public CommandLineUI(TaxConfig config) {
        this.config = config;
        this.calculator = new TaxCalculator(config);
        this.scanner = new java.util.Scanner(System.in);
    }

    /**
     * 启动主菜单循环。
     */
    public void start() {
        boolean running = true;
        while (running) {
            printMenu();
            printConfig(); // 每次菜单后自动展示配置信息
            int choice = readInt("请选择功能：");
            switch (choice) {
                case 1:
                    handleCompute();
                    break;
                case 2:
                    handleSetThreshold();
                    break;
                case 3:
                    handleSetRates();
                    break;
                case 0:
                    running = false;
                    System.out.println("已退出。");
                    break;
                default:
                    System.out.println("无效选择，请重试。");
            }
        }
    }

    private void printMenu() {
        System.out.println("\n============= 个人所得税计算 ==============");
        System.out.println("1. 计算个税");
        System.out.println("2. 设置起征点");
        System.out.println("3. 设置税率");
        System.out.println("0. 退出");
    }

    private void handleCompute() {
        double income = readDouble("请输入当月工资总额：");
        double tax = calculator.computeTax(income);
        System.out.printf("应缴纳个人所得税：%.2f 元\n", tax);
    }

    private void handleSetThreshold() {
        double threshold = readDouble("请输入新的起征点：");
        try {
            config.setThreshold(threshold);
            System.out.println("起征点已更新。");
        } catch (IllegalArgumentException ex) {
            System.out.println("输入无效：" + ex.getMessage());
        }
    }

    private void handleSetRates() {
        System.out.println("当前档位区间和税率如下：");
        TaxBracket[] brackets = config.getBrackets();
        double lower = 0.0;
        for (int i = 0; i < brackets.length; i++) {
            TaxBracket b = brackets[i];
            String upperText = b.getUpperBound() == Double.POSITIVE_INFINITY ? "以上" : String.valueOf(b.getUpperBound());
            System.out.printf("第 %d 档：%.0f - %s，税率 %.2f%%\n", i + 1, lower, upperText, b.getRate() * 100);
            lower = b.getUpperBound();
        }
        System.out.println("请选择设置方式：");
        System.out.println("1. 批量设置所有档位区间和税率");
        System.out.println("2. 单独设置某一档位的区间和税率");
        System.out.print("请输入选项（1或2，其他返回菜单）：");
        String opt = scanner.nextLine().trim();
        if (opt.equals("1")) {
            System.out.println("请输入各档位的上限（以逗号分隔，最后一档填 inf）：");
            String upperLine = scanner.nextLine().trim();
            String[] upperStrs = upperLine.split(",");
            double[] uppers = new double[upperStrs.length];
            for (int i = 0; i < upperStrs.length; i++) {
                if (upperStrs[i].trim().equalsIgnoreCase("inf")) {
                    uppers[i] = Double.POSITIVE_INFINITY;
                } else {
                    try {
                        uppers[i] = Double.parseDouble(upperStrs[i].trim());
                    } catch (NumberFormatException e) {
                        System.out.println("上限输入有误，请重新设置。");
                        return;
                    }
                }
            }
            System.out.println("请输入各档位的税率（以逗号分隔，单位%）：");
            String rateLine = scanner.nextLine().trim();
            String[] rateStrs = rateLine.split(",");
            if (rateStrs.length != uppers.length) {
                System.out.println("税率数量与档位数量不符，请重新设置。");
                return;
            }
            double[] rates = new double[rateStrs.length];
            for (int i = 0; i < rateStrs.length; i++) {
                try {
                    rates[i] = Double.parseDouble(rateStrs[i].trim()) / 100.0;
                } catch (NumberFormatException e) {
                    System.out.println("税率输入有误，请重新设置。");
                    return;
                }
            }
            config.setBrackets(uppers, rates);
            System.out.println("档位区间和税率已批量更新。");
        } else if (opt.equals("2")) {
            System.out.printf("当前共 %d 档，请输入要设置的档位编号（1-%d）：", brackets.length, brackets.length);
            String idxStr = scanner.nextLine().trim();
            int idx;
            try {
                idx = Integer.parseInt(idxStr) - 1;
            } catch (NumberFormatException e) {
                System.out.println("编号输入有误。");
                return;
            }
            if (idx < 0 || idx >= brackets.length) {
                System.out.println("编号超出范围。");
                return;
            }
            System.out.print("请输入该档位新的上限（inf 表示无穷）：");
            String upperStr = scanner.nextLine().trim();
            double upper;
            if (upperStr.equalsIgnoreCase("inf")) {
                upper = Double.POSITIVE_INFINITY;
            } else {
                try {
                    upper = Double.parseDouble(upperStr);
                } catch (NumberFormatException e) {
                    System.out.println("上限输入有误。");
                    return;
                }
            }
            System.out.print("请输入该档位新的税率（单位%）：");
            String rateStr = scanner.nextLine().trim();
            double rate;
            try {
                rate = Double.parseDouble(rateStr) / 100.0;
            } catch (NumberFormatException e) {
                System.out.println("税率输入有误。");
                return;
            }
            // 构造新的档位数组
            double[] uppers = new double[brackets.length];
            double[] rates = new double[brackets.length];
            for (int i = 0; i < brackets.length; i++) {
                uppers[i] = brackets[i].getUpperBound();
                rates[i] = brackets[i].getRate();
            }
            uppers[idx] = upper;
            rates[idx] = rate;
            config.setBrackets(uppers, rates);
            System.out.println("第" + (idx + 1) + "档区间和税率已更新。");
        } else {
            System.out.println("返回主菜单。");
        }
    }

    private void printConfig() {
        System.out.println("============== 当前个税配置 ===============");
        System.out.printf("起征点：%.2f\n", config.getThreshold());
        TaxBracket[] brackets = config.getBrackets();
        double lower = 0.0;
        for (int i = 0; i < brackets.length; i++) {
            TaxBracket b = brackets[i];
            String upperText = b.getUpperBound() == Double.POSITIVE_INFINITY ? "!" : String.format("%.2f", b.getUpperBound());
            System.out.printf("第 %d 档：%8s - %8s  税率：%6.2f%%\n",
                i + 1, String.format("%.2f", lower), upperText, b.getRate() * 100);
            lower = b.getUpperBound();
        }
        System.out.println("===========================================");
    }

    private int readInt(String prompt) {
        while (true) {
            System.out.print(prompt);
            String line = scanner.nextLine().trim();
            try {
                return Integer.parseInt(line);
            } catch (NumberFormatException ex) {
                System.out.println("请输入整数。");
            }
        }
    }

    private double readDouble(String prompt) {
        while (true) {
            System.out.print(prompt);
            String line = scanner.nextLine().trim();
            try {
                return Double.parseDouble(line);
            } catch (NumberFormatException ex) {
                System.out.println("请输入数字。");
            }
        }
    }
}
