bigest I \in M1,M2

I:=empty set;
do {
    build D(I); //binary graph,I,U/I

    //x \in U/I,y \in I
    //x -> y : I+{x}-{y} \in I2
    //y -> x : I+{x}-{y} \in I1

    X1:={z \in U\I :I+{z} \in I1};
    X2:={z \in U\I :I+{z} \in I2};
    P:=shortest path from X1 to X2;
    I:=I xor P;
    // I:=(I or P)-(I and P);  
} while P do not exist