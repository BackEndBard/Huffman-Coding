//huffman hamesha zyada frequency wale ko chota code dene ki try krta hai.

#include<stdio.h>
#include<stdlib.h>

//phase-1
void countFrequency(char *filename)
{
  int freq[256] = {0}; // set counters 0 for all characters.
  FILE *fp = fopen(filename, "r");

  if(!fp)
   printf("\n'%s' file not found or could not be opened. \n", filename);

  char ch;

  while((ch = fgetc(fp)) != EOF)
  {
    freq[ch] += 1; // increase frequency of the characters.
  }

  fclose(fp);
  
  int i;
  
  for(i=0; i< 256; i++)
  {
    if(freq[i] > 0)
    {
      printf("\nCharacter '%c' comes : %d times.\n",i,freq[i]);
    }
  }
}









//phase-2 Min-Heap -: aisa heap joh sabse choti value ko sabse uper rakhega at index 0..!!
typedef struct _Node //har character ke liye ek tree node banega
{
  char data;
  int freq;
  struct _Node *left, *right;
}Node;

typedef struct //joh nodes ko unki frequency ke hisab se line mai rakhega, ye ek array ke jaisa banega..!!
{
  int size;      //abhi kitne nodes hai
  int capacity;  //kisi heap mai max kitne characters aa skte hai.
  Node **array;  //Array of pointers
}MinHeap;

//naya node banane ka function
Node *createNode(char data, int freq)
{
  Node *tmp = (Node *)malloc(sizeof(Node));
  tmp->data = data;
  tmp->freq = freq;
  tmp->left = tmp->right = NULL;
  return tmp;
}

//min-heap banane ka function, ye ek array jaise banega memory mai.
MinHeap *createMinHeap(int capacity)
{ 
  MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
  minHeap->size = 0;
  minHeap->capacity = capacity;
  minHeap->array = (Node **)malloc(sizeof(Node *)*minHeap->capacity);
  return minHeap;
}

//swapping two nodes.
void swapNode(Node **a, Node **b) // direct values ko exchange kr rhe hai using ** ye mtlb pointer of pointer(dereference)
{
  Node *t = *a;
  *a = *b;
  *b = t;
}

//min-heapify - sabse choti value ko array ke top per lana.
void minHeapify(MinHeap *minHeap, int idx)
{
  int smallest = idx; // maan lijiye current idx per joh value hai woh sabse choti value hai.
  int left = 2 * idx + 1; // left half ki size
  int right = 2 * idx + 2; //right half ki size
 
  if(left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) // agr left index per joh value //hai woh agr smallest joh pehle se hi define kiya hai us index per joh value hai usse choti hai, toh smallest mai left //index ko store krdo.
    smallest = left;

  if(right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) //agr right index per joh //value hai woh agr smallest joh pehle se hi define kiya hai us index per joh value hai usse choti hai, toh smallest mai //right index ko store krdo.
    smallest = right;

  if(smallest != idx) // agr smallest badal gya hai yani koi naya node hai joh sabse chota hai, yani joh index ko humne //sabse chota mana starting mai usse bhi chota koi or aa gya hai.
  {
    swapNode(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest); // agle level ke liye phir se check kre by calling recursively function.
  }
}

//heap ke sabse choti value ko nikalna, joh array ke top per hai use.
Node *extractMin(MinHeap *minheap)
{
  Node *tmp = minheap->array[0]; // to use top value later
  minheap->array[0] = minheap->array[minheap->size - 1]; // sabse last node to sabse top per lana.
  --minheap->size; // extract krne ke baad size ko ek kum krna.
  minHeapify(minheap, 0); // aagr sabse last node ko sabse top per lane ke baad woh sabse choti value se bada hua toh //minheap ka rule tut jayega kyoki minheap mai sabse top per sabse choti value hogi isliye minheapify ko call krenge sabse //top value ke liye, taki rule na tute.
  return tmp; // sabse choti value ko return krdenge.
}

//minheap mai do sabse chote node ko jodne ke baad joh naya node banega use wapis heap mai daalna, use heap mai sabse //niche daalenge.
void insertMinHeap(MinHeap *minheap, Node *minHeapNode) // minheapnode - joh naya node hai woh hoga
{
  ++minheap->size; // naya node add krenge isliye size ko badhayenge
  int i = minheap->size - 1;
  while(i && minHeapNode->freq < minheap->array[(i - 1) / 2]->freq) // agr naya node apne parent se chota haia, toh woh 
// node uss parent ko dhakka dega or uski jagah le lega, ye tb tk chalega jab tk woh node sabse top per na pahauch jaye 
// ya use apne se chota koi node na mil jaaye.
  {
    minheap->array[i] = minheap->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }
  minheap->array[i] = minHeapNode;
}










//phase-3  isme hum tree banayenge jisme hum log 2 nodes ko min heap se leke tab tak jodte rahenge jab tak minheap mai //sirf ek node na bach jaaye or woh tree ka root node hoga.

Node *buildHuffmanTree(char data[], int freq[], int size)
{
  Node *left, *right, *top;
  int i;

  //1. pehle ek min-heap banao aur saare characters usme bhar do.
  MinHeap * minHeap = createMinHeap(size);

  for(i=0 ; i<size ; i++)
  {
    insertMinHeap(minHeap, createNode(data[i], freq[i]));
  }
 
  //2. tab tak loop chalao jab tak min heap ka size 1 na ho jaye.
  while(minHeap->size != 1)
  {
    //Do sabse choti frequency wali node ko nikalo.
    left = extractMin(minHeap);
    right = extractMin(minHeap);

    //ek naya "Parent" node banao.
    //'$' ek dummy character hai kyuki ye leaf node nhi hai root node hai tree ka.
    //iski frequency = left + right ki frequency hogi.
    top = createNode('$', left->freq + right->freq);

    top->left = left;
    top->right = right;

    
    //iss naye "Parent" ko wapis min heap mai daal do.
    insertMinHeap(minHeap, top);
  }
  //3. joh aakhri node bacha min heap mai, wahi Huffman Tree ka root hai.
  return extractMin(minHeap);
}









//phase-4 binary codes nikalna. Tree banne ke bad humein root se har character tak jaana hai aur raasta yaad rakhna hai
//left gaye toh 0
//right gaye toh 1

void printCodes(Node *root, int arr[], int top)
{
  //agar left gaye toh array mai 0 daalo, abhi sirf travel kr rhe hai, character nhi mila hai abhi tk.
  if(root->left)
  {
    arr[top] = 0;
    printCodes(root->left, arr, top + 1);
  }

  //agar right gaye toh array mai 1 daalo, abhi sirf travel kr rhe hai, character nhi mila hai abhi tk.
  if(root->right)
  {
    arr[top] = 1;
    printCodes(root->right, arr, top + 1);
  }

  //agar leaf node mil gaya(matlab character mil gaya)
  if(!(root->left) && !(root->right))
  {
    int i;
    printf("%c : ", root->data);
    for(i = 0 ; i < top ; ++i)
    {
      printf("%d", arr[i]);
    }
    printf("\n");
  }
}




int main() {
    int freq[256] = {0};
    char *filename = "test.txt";
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        printf("Error: Could not open %s. Make sure the file exists!\n", filename);
        return 1;
    }

    // 1. Read file and count frequencies
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        freq[(unsigned char)ch]++;
    }
    fclose(fp);

    // 2. Extract only characters that appeared in the file
    char data[256];
    int charFreqs[256];
    int size = 0, i;

    printf("Characters found in '%s':\n", filename);
    printf("-------------------------------\n");
    for (i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            charFreqs[size] = freq[i];
            
            if (i == '\n') printf(" [\\n] freq: %d\n", freq[i]);
            else if (i == ' ') printf(" [Spc] freq: %d\n", freq[i]);
            else printf(" [%c]   freq: %d\n", i, freq[i]);
            
            size++;
        }
    }

    if (size == 0) {
        printf("The file is empty.\n");
        return 0;
    }

    // 3. Build the Tree
    Node *root = buildHuffmanTree(data, charFreqs, size);

    // 4. Generate and print the binary codes
    int code_arr[100], top = 0;
    printf("\nGenerated Huffman Codes:\n");
    printf("-------------------------------\n");
    printCodes(root, code_arr, top);
    printf("-------------------------------\n");

    return 0;
}