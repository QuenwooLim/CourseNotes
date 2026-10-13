/**
 * Nested scope for Oberon-0 symbol table.
 * Each scope tracks local symbols and delegates to its parent for outer names.
 */
public class Scope {
    private final Scope parent;
    private final java.util.Map<String, String> symbols = new java.util.LinkedHashMap<>();

    public Scope() { this(null); }
    public Scope(Scope parent) { this.parent = parent; }

    /** Declare a name in this scope. Returns false if already declared here. */
    public boolean define(String name, String type) {
        String key = name.toLowerCase();
        if (symbols.containsKey(key)) return false;
        symbols.put(key, type);
        return true;
    }

    /** Look up a name, walking up the scope chain. Returns null if not found. */
    public String lookup(String name) {
        String key = name.toLowerCase();
        String t = symbols.get(key);
        if (t != null) return t;
        if (parent != null) return parent.lookup(name);
        return null;
    }

    /** Create a child scope inheriting from this one. */
    public Scope child() { return new Scope(this); }
}
