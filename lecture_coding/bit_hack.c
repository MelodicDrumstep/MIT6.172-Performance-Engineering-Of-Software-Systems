inline int LSB(int x)
{
    return x & (-x);
    //This will return the least significant bit
}