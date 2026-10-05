// 結構體 X
struct DataX {
    char a;
    short b;
    char c;
    int d;
    //a+p+b+b+c+p+p+p+d+d+d+d=12bytes
};

// 結構體 Y
struct DataY {
    int d;
    short b;
    char a;
    char c;
    //d+d+d+d+b+b+a+c=8bytes
};