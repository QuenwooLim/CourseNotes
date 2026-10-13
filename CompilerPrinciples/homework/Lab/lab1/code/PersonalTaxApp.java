
/**
 * 个人所得税计算程序主入口类。
 * <p>
 * 启动配置和命令行界面。
 * </p>
 * @author 林宏宇
 * @version 1.0
 */
public class PersonalTaxApp {

    /**
     * 程序主方法，启动个税计算系统。
     * @param args 命令行参数
     */
    public static void main(String[] args) {
        TaxConfig config = new TaxConfig();
        CommandLineUI ui = new CommandLineUI(config);
        ui.start();
    }
}
