struct Packet {
    char cmd;
    int data;
    short checksum;
    //cmd+p+p+p+data+data+data+data+checksum+checksum+p+p=12bytes
};