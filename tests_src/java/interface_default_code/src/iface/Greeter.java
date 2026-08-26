package iface;

/** An interface with a default method, so that Impl can call it with invokespecial. */
public interface Greeter {
    default String greet() {
        return "hi";
    }
}
