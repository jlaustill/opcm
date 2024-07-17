//
// Create by jlaustill on 7/6/24
//


class BlinkOutput {
    public:
        static void initialize();
        static void blink();

    private:
        static long lastBlink;
};

