class ReorderBuffer {

    int** buffer;
    int size;
    int head;
    int tail;

    public:
    ReorderBuffer(int size) : 
        size(size)
    {
    	//dynamically allocated a 2d array to the read and write buffer
        buffer = new int*[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = new int[4];
        }
        //initialise all elements of the read and write buffer to zero
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < 4; j++) {
                buffer[i][j] = 0;
            }
        }
    }   
};