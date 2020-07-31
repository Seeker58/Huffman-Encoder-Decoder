#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Node{
      public:
             Node(char temp);
             Node(Node *leftChild, Node *rightChild);
             Node(char valueInput, unsigned int frequencyInput);
             void incrementFrequency();
             char getValue();
             unsigned int getFrequency();
             void setOrder(int order);

             // This pointer will be used in the link list
             Node *next;
             Node *previous;

             // These pointers will be used in the tree
             Node *right;
             Node *left;

      private:
              char value;
              unsigned int frequency;

};

Node::Node(char temp){
     value = temp;
     frequency = 1;
     right = NULL;
     left = NULL;
     next = NULL;
     previous = NULL;
}

Node::Node(Node *leftChild, Node *rightChild){
     value = 0;
     frequency = leftChild->getFrequency() + rightChild->getFrequency();
     right = rightChild;
     left = leftChild;
     next = NULL;
     previous = NULL;
}

Node::Node(char valueInput, unsigned int frequencyInput){
     value = valueInput;
     frequency = frequencyInput;
     right = NULL;
     left = NULL;
     next = NULL;
     previous = NULL;
}

void Node::incrementFrequency(){
     frequency++;
}

char Node::getValue(){
     return value;
}

unsigned int Node::getFrequency(){
    return frequency;
}

void Node::setOrder(int order){
     frequency = order;
     return;
}


void incrementNode(Node *currentNode, char input);
Node* convertListToTree(Node *listRoot);
void deleteTree(Node *root);
void printLinkList(Node *root);
void storeCodes(Node *root, unsigned char codeArray[255][255], unsigned char temp[255], int depth);
void printTree(Node *root, int code, int depth);
void printCode(int code, int depth);
int pow(int base, int exponent);
void encode(ifstream *inputFile, ofstream *outputFile, unsigned char codeArray[255][255], Node *root);
void numberNodes(Node *root, int *i);
void outputNodes(Node *root, ofstream *outputFile);
void printEncoding(unsigned char charOutput[8], ofstream *outputFile, ofstream *logFile);
void decode();
void insertNode(Node* root, Node *tempNode);
void convert(unsigned
int temp1, int instructionBuffer[8]);



int main(int argc, char *argv[]){

    char textFile[200];
    unsigned int i;
    int j;
    unsigned char tempChar;
    char a, b, c;
    unsigned char codeArray[255][255];
    unsigned char tempArray[255];

    unsigned int tempInt;
    Node *root = NULL;
    ifstream *inputFile;
    ofstream *outputFile;


    cout << "Please enter the name of the text file you would like to encode: ";
    cin >> textFile;

    inputFile = new ifstream(textFile);

    if(inputFile->fail())
    {
          cerr << "\nERROR: Could not open the file requested.  Exiting program...\n";
          system("PAUSE");
          return 0;
    }

    // Input the first character, double check that its not an empty file, and
    // then, if it isnt, create a Node for that character and make it the root of
    // what will become a linked list.  Then grab another character to prepare
    // for the while loop.
    tempChar = inputFile->get();
    if(tempChar != '\0'){
         root = new Node(tempChar);
         tempChar = inputFile->get();
    }

    // Input characters from the file and either increment the frequency value of
    // the corresponding node or tack a new Node corresponding to that character
    // onto the end of the list if it doesnt exist.
    while(tempChar != 255){
         incrementNode(root, tempChar);
         tempChar = inputFile->get();
    }

    // Convert the linked list of nodes to a Huffman tree and return the root
    root = convertListToTree(root);

    // Output the character value, its frequency, and the corresponding code
    cout << "Value\tFrequency\tCode\n\n";
    storeCodes(root, codeArray, tempArray, 0);

    // Close the file and delete the file pointer
    inputFile->close();
    delete(inputFile);

    /**************************** Challenge part *******************************/
    // At this point the file has been read, a Huffman tree generated, an array//
    // of codes created, and then the file was closed.                         //
    //                                                                         //
    // The file will now be reopened and encoded as the file "encode.txt"      //
    /***************************************************************************/

    // Re-open the file and the file to hold the encoding
    inputFile = new ifstream(textFile);
    outputFile = new ofstream("encode.txt");

    // Encode the values of inputFile into outputFile using
    // the codeArray and root of the Huffman tree
    encode(inputFile, outputFile, codeArray, root);

    // Close the two files and delete the Huffman tree
    outputFile->close();
    inputFile->close();
    deleteTree(root);

    // Using the encoding just created, re-create the original txt file
    decode();


    system("PAUSE");
    return EXIT_SUCCESS;
}


/*----------------------------------------------------------------------------*/

// Goes through the linked list recursively and either increments the frequency
// of the node with the same value as the input -or- it makes a new node with a
// frequency of '1' and tacks it onto the end of the list.
void incrementNode(Node *currentNode, char input){
     if(currentNode->getValue() == input){
          currentNode->incrementFrequency();
     }
     else if(currentNode->next == NULL){
          currentNode->next = new Node(input);
          currentNode->next->previous = currentNode;
     }
     else{
          incrementNode(currentNode->next, input);
     }
     return;
}

/*----------------------------------------------------------------------------*/

// Accepts the root to a linked list of Nodes that also happen to have left and
// right pointers so they can seamlessly be added to a binary tree at the same
// time.  It then bunches them by lowest frequency until it only has one left in
// the list.
Node *convertListToTree(Node *listRoot){
     // These two Node pointers will point to the lowest frequency nodes
     // as lower ones are searched for.
     Node *lowestFreq, *lowestFreq2;

     Node *tempNode = NULL;

     // Be sure there are at least two elements left in the list
     while(listRoot->next != NULL){

     // Initialize the two lowest frequency pointers as the first two nodes
          if(listRoot->getFrequency() > listRoot->next->getFrequency()){
               lowestFreq = listRoot->next;
               lowestFreq2 = listRoot;
          }
          else{
               lowestFreq = listRoot;
               lowestFreq2 = listRoot->next;
          }
     // Initialize tempNode as the 2nd Node's next pointer
          tempNode = listRoot->next->next;
          while(tempNode != NULL){
               if(tempNode->getFrequency() <= lowestFreq2->getFrequency()){
                    if(tempNode->getFrequency() <= lowestFreq->getFrequency()){
                         lowestFreq2 = lowestFreq;
                         lowestFreq = tempNode;
                    }
                    else{
                         lowestFreq2 = tempNode;
                    }
               }
               tempNode = tempNode->next;
          }


     // Remove the two lowest frequency nodes from the list
          if(lowestFreq->next != NULL){
               lowestFreq->next->previous = lowestFreq->previous;
          }

          if(lowestFreq->previous != NULL){
               lowestFreq->previous->next = lowestFreq->next;
          }
          else{
               listRoot = lowestFreq->next;
          }

          if(lowestFreq2->next != NULL){
               lowestFreq2->next->previous = lowestFreq2->previous;
          }

          if(lowestFreq2->previous != NULL){
               lowestFreq2->previous->next = lowestFreq2->next;
          }
          else{
               listRoot = lowestFreq2->next;
          }

          lowestFreq->next = NULL;
          lowestFreq->previous = NULL;
          lowestFreq2->next = NULL;
          lowestFreq2->previous = NULL;

       // Create a new node with those two nodes as children and stick it at
       // the front of the link list.
          tempNode = new Node(lowestFreq, lowestFreq2);
          tempNode->next = listRoot;
          if(listRoot != NULL){
               listRoot->previous = tempNode;
          }
          listRoot = tempNode;
          /*DELETE THIS LATER
          cout << "\nWell, we just removed the " << lowestFreq->getValue();
          cout << " and the " << lowestFreq2->getValue() << " node.\nEnd result:\n";

          printLinkList(listRoot);

          system("PAUSE");*/

     }


     return listRoot;
}

/*----------------------------------------------------------------------------*/

// Deletes the binary search tree passed to it
void deleteTree(Node *root){
     if(root == NULL){
             return;
     }
     deleteTree(root->left);
     deleteTree(root->right);
     delete(root);
     return;
}

/*----------------------------------------------------------------------------*/
// Prints the values and frequencies in a link list of Nodes
// This function isn't actually used anymore and was more for testing purposes
void printLinkList(Node *root){
     if(root == NULL){
             return;
     }
     cout << root->getValue() << " : " << root->getFrequency() << endl;
     printLinkList(root->next);
}

/*----------------------------------------------------------------------------*/
// Prints the value, frequency, and corresponding code using an integer
// and bit shifting
void printTree(Node *root, int code, int depth){
     if(root == NULL){return;}

     printTree(root->left, (code * 2), depth + 1 );
     if(root->getValue() != char(0)){
          cout << root->getValue() << "\t" << root->getFrequency();
          cout << "\t\t";
          printCode(code, depth);
          cout << endl;
     }
     printTree(root->right, ((code*2)+1), depth + 1 );

}

/*----------------------------------------------------------------------------*/

void storeCodes(Node *root, unsigned char codeArray[255][255], unsigned char temp[255], int depth){
     if(root == NULL){return;}

     int value;
     int i;
     value = int(root->getValue());

     if(root->getValue() != char(0)){
          for(i = 0; i < depth; i++){
               codeArray[value][i] = temp[i];
          }
          codeArray[value][i] = '\0';

          cout << root->getValue() << "\t" << root->getFrequency() << "\t\t";
          cout << codeArray[int(root->getValue())] << endl;
     }
     temp[depth+1] = '\0';

     temp[depth] = '0';
     storeCodes(root->left, codeArray, temp, depth+1);

     temp[depth] = '1';
     storeCodes(root->right, codeArray, temp, depth+1);
}

/*----------------------------------------------------------------------------*/
// Prints a node's code in binary given its depth and value in base 10
void printCode(int code, int depth){

     int temp;
     temp = code;


     if(depth > 0){
              depth = pow(2, depth-1);
     }

     while(depth > 0){

                 if(temp >= depth){
                         cout << "1";
                         temp -= depth;
                 }
                 else{
                      cout << "0";
                 }
                 depth /= 2;
     }

     return;

}

/*----------------------------------------------------------------------------*/
// A simple power function for integers and exponents that are only positive
int pow(int base, int exponent){
    if(exponent == 0){
                return 1;
    }
    return base*pow(base, --exponent);
}

/*----------------------------------------------------------------------------*/
// This file outputs to the file "encode.txt" the encoded version of the file
// passed to it as inputFile.  It first outputs the # of nodes in the huffman tree
// passed to it for encoding purposes, followed by an endline.  It then prints,
// in a preorder traversal, the character value of the node followed by the
// order of that node in an inorder traversal (also as a character).  Then, using
// the array of codes passed to it, it packs the code corresponding to the text of
// the input file into 8 bit segments and outputs them as character values to
// encoded.txt.  The last character of the file corresponds to how many bits should
// be used as instructions in the 2nd to last character of the file.
void encode(ifstream *inputFile, ofstream *outputFile, unsigned char codeArray[255][255], Node *root){

     int i = 1, j;
     int temp;
     int digits = 0;
     int arrayLoc;
     unsigned char charOutput[8] = "";
     unsigned char val;
     ofstream *logFile = new ofstream("encodinglog.txt");

     // Go through the Nodes inorder and change their frequency value to correspond to
     // their order in the tree for an inorder traversal so the tree can be recreated
     // for decoding purposes
     numberNodes(root, &i);

     // i gets incremented one extra time and therefore needs to be decremented to
     // correspond to the amount of nodes in the header.  If it is greater than 26
     // it got incremented yet another extra time to avoid outputting the character
     // 26 and, therefore, needs to be decremented one more time
     if(i-- > 26){ i--; }

     // Output the Node Count to the file
     *outputFile << i << endl;

     // Output value and order of each node in a preorder traversal
     // so the Huffman tree can be re-created with ease upon decoding

     outputNodes(root, outputFile);

     // Read in values from the input file until you hit the end of file

     temp = 0;
     while((val = inputFile->get()) != 255){
          // find the length of the code
          digits = 0;
          while(codeArray[int(val)][digits] != '\0'){
               digits++;
          }

          arrayLoc = 0;
          while (arrayLoc < digits){
                charOutput[temp] = codeArray[int(val)][arrayLoc];
                temp++;
                charOutput[temp] = '\0';
                arrayLoc++;

                // You can't output the code 11111111 (8 1's) as a character because it
                // corresponds to EOF (End of File) - so if you run into the code
                // of 1111111 (7 1's), add a 0 on the end and output that instead.
                // The decoder function will know that when it reads in the char
                // 11111110 that it actually corresponds to the instruction of 7 1's
                if(temp == 7){
                        if(charOutput[0] == '1' && charOutput[1] == '1' &&
                        charOutput[2] == '1' && charOutput[3] == '1' &&
                        charOutput[4] == '1' && charOutput[5] == '1' &&
                        charOutput[6] == '1'){
                                      charOutput[7] = '0';
                                      printEncoding(charOutput, outputFile, logFile);
                                      temp = 0;
                                      charOutput[0] = '\0';
                        }
                        // You also can't output the code 00011010 (26) because it
                        // corresponds to some weird command.  So if you get 0001101
                        // stop, tack a 1 on the end, and output that instead - the
                        // decoder will know how to handle an input of '27'
                        if(charOutput[0] == '0' && charOutput[1] == '0' &&
                        charOutput[2] == '0' && charOutput[3] == '1' &&
                        charOutput[4] == '1' && charOutput[5] == '0' &&
                        charOutput[6] == '1'){
                                      charOutput[7] = '1';
                                      printEncoding(charOutput, outputFile, logFile);
                                      temp = 0;
                                      charOutput[0] = '\0';
                        }
                }
                if(temp == 8){
                        printEncoding(charOutput, outputFile, logFile);
                        temp = 0;
                        charOutput[0] = '\0';
                }
          }
     }

     // Encode what is left of the buffer
     j = 0;
     if(charOutput[0] != '\0'){
          j += 128*(int(charOutput[0])-'0');
               if(charOutput[1] != '\0'){
                    j += 64*(int(charOutput[1])-'0');
                         if(charOutput[2] != '\0'){
                              j += 32*(int(charOutput[2])-'0');
                                   if(charOutput[3] != '\0'){
                                        j += 16*(int(charOutput[3])-'0');
                                             if(charOutput[4] != '\0'){
                                                  j += 8*(int(charOutput[4])-'0');
                                                       if(charOutput[5] != '\0'){
                                                           j += 4*(int(charOutput[5])-'0');
                                                                if(charOutput[6] != '\0'){
                                                                     j += 2*(int(charOutput[6])-'0');
                                                                          if(charOutput[7] != '\0'){
                                                                               j += (int(charOutput[7])-'0');
                                                                          }
                                                                }
                                                       }
                                             }
                                   }
                         }
               }
     }
     // Once again, mysterious char '26' cannot be outputted
     if(j == 26){
          j++;
     }
     outputFile->put(char(j));
     // Print how many bits to use
     outputFile->put(char(temp));

     //DELETE LATER
     logFile->close();

}

/*----------------------------------------------------------------------------*/
// This function goes through the nodes of the huffman tree and stores their
// order (inorder traversal) in their frequency value (as its no longer needed
// once the tree is created).

void numberNodes(Node *root, int *i){
     if(root == NULL){
          return;
     }
     numberNodes(root->left, i);
     // Outputting the #26 as a character results in weird stuff happening so
     // dont mess with it - just skip it
     if((*i) == 26){
             (*i)++;
     }
     root->setOrder((*i));
     (*i)++;
     numberNodes(root->right, i);
}

/*----------------------------------------------------------------------------*/
// This function goes through the nodes in a preorder traversal and outputs their
// character value followed immediately by their inorder value in the tree so that
// the tree can be accurately recreated upon decoding

void outputNodes(Node *root, ofstream *outputFile){
     if(root == NULL){return;}
     outputFile->put(root->getValue());
     outputFile->put((unsigned char)(root->getFrequency()));
     outputNodes(root->left, outputFile);
     outputNodes(root->right, outputFile);
}

/*----------------------------------------------------------------------------*/
// This function inputs a length 8 character array of '1' and '0' (binary),
// converts it to a decimal value, and then prints its character value to the
// file holding the encoding.  It also prints the corresponding binary, integer,
// and character values to a logfile for review.
void printEncoding(unsigned char charOutput[8], ofstream *outputFile, ofstream *logFile){
     int temp = 0;

     temp = (int(charOutput[7])-'0') + 2*(int(charOutput[6])-'0') + 4*(int(charOutput[5])-'0') + 8*(int(charOutput[4])-'0') +
     16*(int(charOutput[3])-'0') + 32*(int(charOutput[2])-'0') + 64*(int(charOutput[1])-'0') + 128*(int(charOutput[0])-'0');

     *logFile << "\nPrinting the code " << charOutput[0] << charOutput[1] << charOutput[2] << charOutput[3];
     *logFile << charOutput[4] << charOutput[5] << charOutput[6] << charOutput[7] << " as int value " << temp;
     *logFile << " which, in character form, is: " << char(temp) << endl;

     outputFile->put(char(temp));
     return;
}

/*----------------------------------------------------------------------------*/
// This function opens the encode.txt file, creates a huffman tree from the
// header, and then proceeds to traverse the tree it created using the binary
// values of the characters it reads in as traversal instructions.  It outputs
// the characters to the decode.txt file, re-creating the initially encoded file
// entered by the user at the beginning of main()
void decode(){
     Node *root = NULL;
     Node *tempNode = NULL;
     int instructionBuffer[8];
     int bufferLoc;
     unsigned int temp1;
     unsigned int temp2;
     unsigned char temp3;

     ifstream *inputFile = new ifstream("encode.txt");
     ofstream *outputFile = new ofstream("decode.txt");

     unsigned int nodeCount;

     // find out how many nodes are in the tree, as in how large the header is,
     // as in find out how many chars correspond to node values and order #'s
     // and when to actually start reading code
     *inputFile >> nodeCount;
     // After the nodeCount there is an endline that corresponds to nothing except
     // to allow the *inputFile >> nodeCount; line to know where the nodeCount int
     // stops - so grab the '\n' (endl) character and do nothing with it so it
     // doesnt get confused with node values
     temp3 = inputFile->get();

     // Read in the nodes and re-create the Huffman tree
     while(nodeCount > 0){
          tempNode = new Node(inputFile->get(), (unsigned int)(inputFile->get()));
          if(root == NULL){
                  root = tempNode;
          }
          else{
               insertNode(root, tempNode);
          }
          nodeCount--;
     }


     // Grab the initial characters from the file - there will always be at
     // least two
     tempNode = root;
     bufferLoc = 0;
     temp3 = inputFile->get();
     temp1 = int(temp3);
     temp3 = inputFile->get();
     temp2 = int(temp3);

     while( (temp3 = inputFile->get()) != 255 ){
            // If the character that is read in corresponds to the code 11111110,
            // then it actually is supposed to correlate to the instruction of 7 0's
            // as 8 1's cannot be outputted to the file.  Therefore change it to equal
            // 01111111 and increment the bufferLoc so it skips the initial 0.
            if( temp1 == 254 ){
                temp1 = 127;
                bufferLoc++;
            }
            // If the character that is read in corresponds to the code 00011011,
            // it is actually supposed to correlate to 0001101 as '26' (00011010)
            // cannot be read from a file for mysterious reasons
            if( temp1 == 27 ){
                temp1 = 13;
                bufferLoc++;
            }

            convert(temp1, instructionBuffer);

            temp1 = temp2;
            temp2 = int(temp3);

            while(bufferLoc < 8){
                 if(tempNode->left == NULL && tempNode->right == NULL){
                      outputFile->put(tempNode->getValue());
                      tempNode = root;
                 }
                 else if(instructionBuffer[bufferLoc] == 1){
                      tempNode = tempNode->right;
                      bufferLoc++;
                 }
                 else{
                      tempNode = tempNode->left;
                      bufferLoc++;
                 }
            }
            bufferLoc = 0;
     }

     // You're now left with temp1 = last instruction and temp2 = how many bits to use of that instruction

     convert(temp1, instructionBuffer);
     while(bufferLoc < temp2){
          if(tempNode->left == NULL && tempNode->right == NULL){
               outputFile->put(tempNode->getValue());
               tempNode = root;
          }
          else if(instructionBuffer[bufferLoc] == 1){
               tempNode = tempNode->right;
               bufferLoc++;
          }
          else{
               tempNode = tempNode->left;
               bufferLoc++;
          }
     }

     if(temp2 != 0){
              if(tempNode->left != NULL || tempNode->right != NULL){
                   cout << "\n\nERROR: The file was improperly encoded.\n\n";
              }
              outputFile->put(tempNode->getValue());
     }
     outputFile->close();
}

/*----------------------------------------------------------------------------*/
// This function inserts the node tempNode into the huffman tree pointed to by
// root
void insertNode(Node *root, Node *tempNode){
     if(tempNode->getFrequency() < root->getFrequency()){
          if(root->left == NULL){
               root->left = tempNode;
          }
          else{
               insertNode(root->left, tempNode);
          }
     }
     else{
          if(root->right == NULL){
               root->right = tempNode;
          }
          else{
               insertNode(root->right, tempNode);
          }
     }
}

/*----------------------------------------------------------------------------*/
// This function is passed an 8 bit int and converts it to its binary value that
// is stored in instructionBuffer[8]
void convert(unsigned int temp1, int instructionBuffer[8]){
     unsigned int temp, i, powerof2;
     temp = temp1;
     powerof2 = 128;

     for(i = 0; i < 8; i++){
           if(temp >= powerof2){
                   instructionBuffer[i] = 1;
                   temp -= powerof2;
                   powerof2 /= 2;
           }
           else{
                instructionBuffer[i] = 0;
                powerof2 /= 2;
           }
     }

}
