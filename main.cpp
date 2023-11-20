#include <bits/stdc++.h>
#define MIN_HEAP 1
#define MAX_HEAP 0


using namespace std;

int display_some_things(string text_to_display , bool is_get_input)
{
    /*
        this is a simple function to display text on the console screen . this will automatically clear
        the screen and let the user pause to read text . It can also be use to get an input while display some thing.
    */
    system("clear");
    cout << text_to_display << endl;

    if(is_get_input)
    {
        int input = 0;
        cin >> input;

        system("clear");

        return input;
    }

    cout << "type a character and press enter to continue: ";
    
    // this just a tmp variable to receive a input from the user
    // we have to use cin.ignore() to clear the stdin

    string tmp = "";
    cin >> tmp;
    cin.ignore();

    // getchar();
    // cin.ignore();
    // fflush(stdin);

    system("clear");

    return 0;
}

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

class HeapNode
{
    public:
        int value;
        HeapNode* left_node;
        HeapNode* right_node;
        int subtree_size;

        HeapNode(int value = 0 ,HeapNode* left_node = nullptr ,HeapNode* right_node = nullptr)
        {
            this->value = value;
            this->left_node = left_node;
            this->right_node = right_node;
            
            subtree_size = 1;

            if(left_node != nullptr)
            {
                subtree_size += left_node->subtree_size;
            }

            if(right_node != nullptr)
            {
                subtree_size += right_node->subtree_size;
            }
        }

};

class heap
{
    private:
        HeapNode* root;
        int status;
        int size;
        int found;
        vector<int> delete_path;
        const int LEFT = 0;
        const int RIGHT = 1;


        HeapNode* delete_value_from_current_node(HeapNode* root , int value_to_delete)
        {

            // if this node is not the value to delete , then do nothing
            if(root->value  != value_to_delete) return root;

            /*
                this function share the same concept as delete a value from binary tree.

                if the delete node have no children , then return null 

                if the delete node have 1 children , then return the children 

                if the node have 2 children , then we manage to swap the values with the children 
                that have the most subtree count and recurion delete_value_from_current_node(children)
            */
            if(root->left_node ==  nullptr and root->right_node == nullptr)
            {
                // deallocate the memory since c++ doesn't support garbage collection
                delete root;
                return nullptr;
            }
        
            if( root-> left_node == nullptr)  
            {
                HeapNode* tmp = root->right_node;
                delete root;
                return tmp;
            }

            if( root-> right_node == nullptr)
            {
                HeapNode* tmp = root->left_node;
                delete root;
                return tmp;
            }
            //we will swap the delete value with the subtree that have the most size , then 
            // erase it from the heap tree to protect the balance of the heap tree
            
            //we know for sure that a node in this subtree will be deleted
            root->subtree_size -= 1;
            
            if(root->left_node->subtree_size > root->right_node->subtree_size)
            {
                int tmp = root->value;
                root->value = root->left_node->value;
                root->left_node->value = tmp;

                root->left_node = delete_value_from_current_node(root->left_node , value_to_delete);

            }
            else
            {
                int tmp = root->value;
                root->value = root->right_node->value;
                root->right_node->value = tmp;

                root->right_node = delete_value_from_current_node(root->right_node , value_to_delete);
            }
            
            // we heapify to make sure that the tree focus the concepts of heap
            heapify(root,root->left_node,root->right_node);

            return root;
        }

        void find_node_and_construct_delete_path(HeapNode* root , int value , vector<int>& path)
        {
            /*
                this method work with the same concept as the find_node 
                but this will track the searching path for deletion
            */
            if(root == nullptr or found == true)return ;

            if(root->value == value) 
            {
                found = true;
                delete_path = path;
            }
        
            if(root == nullptr or found == true)return ;

            if(root->value > value and status == MIN_HEAP) return;
            if(root->value < value and status == MAX_HEAP) return;

            // create a path for left child and remove the current added 
            path.push_back(LEFT);
            find_node_and_construct_delete_path(root->left_node, value , path);
            path.pop_back();

            // create a path for right child and remove the current added
            path.push_back(RIGHT);
            find_node_and_construct_delete_path(root->right_node, value , path);
            path.pop_back();

        }

        void max_heapify(HeapNode* root, HeapNode* left_node , HeapNode* right_node)
        {
            if(left_node != nullptr)
            {
                if(left_node->value > root->value)
                {
                    int tmp = root->value;
                    root->value = left_node->value;
                    left_node->value = tmp;
                } 
            }

            if(right_node != nullptr)
            {
                if(right_node->value > root->value)
                {
                    int tmp = root->value;
                    root->value = right_node->value;
                    right_node->value = tmp;
                }
            }
        }

        void min_heapify(HeapNode* root, HeapNode* left_node , HeapNode* right_node)
        {
            if(left_node != nullptr)
            {
                if(left_node->value < root->value)
                {
                    int tmp = root->value;
                    root->value = left_node->value;
                    left_node->value = tmp;
                }
            }

            if(right_node != nullptr)
            {
                if(right_node->value < root->value)
                {
                    int tmp = root->value;
                    root->value = right_node->value;
                    right_node->value = tmp;
                }
            }
        }

        void heapify(HeapNode* root , HeapNode* left_node , HeapNode* right_node)
        {
            // check the status of the heap , then choosing the right heapify method
            if(status == MAX_HEAP)
            {
                max_heapify(root , left_node , right_node);
            }
            else
            {
                min_heapify(root , left_node , right_node);
            }
        }

        HeapNode* binary_insert_and_heapify(HeapNode* root ,HeapNode* new_node)
        {
            /*
            this function will insert the new node into the heap with the same concept as 
            inserting a node into binary tree then heapify the heap 

            we will use the number of children in each subtree to determine 
            the new node will be inseted left or right 
            
            this way , we can manage balance of the tree

            */

            // if this is null , then this is where new node is inserted (a new leaf)
            if(root == nullptr) return new_node;

            // increase 1 since we know for sure that a new node will be inserted
            // into this subtree.
            root->subtree_size += 1;

            // this will be the leaf node
            if(root->right_node == nullptr)
            {   
                root->right_node =binary_insert_and_heapify(root->right_node,new_node);

                heapify(root , root->left_node , root->right_node);

                return root;
            }

            // this will be the leaf node
            if(root->left_node == nullptr)
            {
                root->left_node =binary_insert_and_heapify(root->left_node,new_node);

                heapify(root , root->left_node , root->right_node);

                return root;
            }

            // therefor we can manage the balance of the tree.

            if(root->right_node->subtree_size > root->left_node->subtree_size) 
            {
                root->left_node =binary_insert_and_heapify(root->left_node,new_node);
            }
            else
            {
                root->right_node =binary_insert_and_heapify(root->right_node,new_node);
            }

            heapify(root , root->left_node , root->right_node);

            return root;
        }

        void find_node(int value ,HeapNode* root)
        {
            //stop when we have found the value or this is a null node
            if(root == nullptr or found == true )return;

            if(root->value == value)
            {
                found = true;
                return ;
            }
            // in min heap , the children node will have bigger value than the current node
            if(root->value > value and status == MIN_HEAP) return;

            // in max heap , the children node will have smaller value than the current node
            if(root->value < value and status == MAX_HEAP) return;

            //search for left tree
            find_node(value , root->left_node);
            //search for right tree
            find_node(value , root->right_node);

            
        }


        int recursion_print(HeapNode* root , int x , int y )
        {
            /*  the concept of this function is pretty easy to understand.we will use the inorder traversal and printing the heap tree

                imagine the height of the tree is the y coordinate
                
                therefore , each time we recursion to a nother child node , we increase y one ( but it will return to normal when we done recursion to that node)

                but the x coordinate is represent the lenght of the value . therefore , no matter the level  of  tree height , once
                we print some things down , we must increase the x value entenaly and not restore it even we use recursion.

                that is the reason why my function returns int ( the lenght of x) to make sure that if we modify the x , it will stay the same.


                EX :
                y                 
                1                1123809214
                2     192831                    1029301920
                3 19312      0912           0129          20134

                we can see that y is changing but x won't only increase )

                
            */


            if(root == nullptr)return x;

            // convert to string to know the increase size  x coordinate
            string text = to_string(root->value);

            // we advance the level of the y by one but when it's done this recursively , it will return to normal
            // while x is different (we keep the modified value)
            // this is a inorder traversal
            x = recursion_print(root->left_node , x , y + 1);
            
            gotoxy(x , y);
            cout << text;
            x += text.size() + 1;

            x = recursion_print(root->right_node , x , y + 1);

            // return the modified value of x 
            return x;
        }

    public:
        heap(int status = MIN_HEAP )
        {
            // normal constructor
            this->status = status;
            size = 0;
            root = nullptr;
        }

        heap(vector<int> array , int status = MIN_HEAP)
        {
            /*
                contructor to turn array into a heap .
                this will not follow the concept of normal heap in the internet but will follow my
                special algorithm
            */
            this->status = status;
            size = array.size();
            root = nullptr;

            for(auto value : array)
            {
                insert(value);
            }
        }

        void insert(int value)
        {
            /*
                this function push new value to heap using binary_insert_and_heapify helper method.

                the reason why we usebinary_insert_and_heapify is that we won't have to 
                rebalance the heap every time we call this function.
            */
            HeapNode* new_node = new HeapNode(value);



            root =binary_insert_and_heapify(root, new_node);


        }

        int top()
        {
            return root->value;
        }

        bool find(int value)
        {
            found = false;
            find_node(value , root);

            if(found)
            {
                // return the found status to false to prevent bugs
                found = false;
                return true;
            }
            return false;
        }
    
        void erase(int value_to_delete)
        {
            // if the delete node is the root value, we will have a different method to erase it
            if(root->value == value_to_delete)
            {
                root = delete_value_from_current_node(root , value_to_delete);

                return;
            } 

            // set the found status to false to start finding the value
            found = false;
            
            HeapNode* tmp = root;

            vector<int> tmp_path;   

            // set the status found to false before searching 
            found = false;
            // we find the value to delete and and keep track of it.
            find_node_and_construct_delete_path(root , value_to_delete , tmp_path);

            if(found == false){
                cout << "value not exists! \n";
                return;            
            }

            // reset the found status to prevent bugs
            found = false;
            
            HeapNode* last_node_before_delete_node = root;

            // traverse through the tree to the value to delete .
            // the reason why we have to travel like this is because we have to decrease the 
            // subtree_size that the  ancestor of the delete value have .

            // if we can't find the value , then the delete path will be empty
            for(int i = 0 ; i < delete_path.size() ; i ++)
            {
                tmp->subtree_size -= 1;
                if(delete_path.at(i) == LEFT)
                {
                    last_node_before_delete_node = tmp;
                    tmp = tmp->left_node;
                }
                else
                {
                    last_node_before_delete_node = tmp;
                    tmp = tmp->right_node;
                }   
            }


            // reset the delete_path
            delete_path.clear();

            // if the deleted value is the left node 
            if(last_node_before_delete_node->left_node == tmp)
            {
                last_node_before_delete_node->left_node = delete_value_from_current_node(tmp , value_to_delete);
            }

            // if the deleted value is the right node
            if(last_node_before_delete_node->right_node == tmp)
            {
                last_node_before_delete_node->right_node = delete_value_from_current_node(tmp , value_to_delete);
            }

            return;
        }
        
        void print_heap()
        {
            recursion_print(root , 1, 1);
        }

};

class MENU
{
    private:
        const string greeting_text = R"(
        *************************
        *                       *
        *    MAX / MIN HEAP     *
        *                       *
        *            by dobach  *
        *************************
    )";

        const string instruction = R"(
        this is a simple program that visualize the min/max heap
        user can add , find , remove , find the top element or print the heap (binary tree format) to the console.

        enjoy using it !!! \n
    )";


        const string option_text = R"(
    *********************
    *   1 insert        * 
    *   2 find          *
    *   3 top           *
    *   4 erase         *
    *   5 visualize     *
    *   6 exit          *
    *********************

    your choice: 
)";
        
        const string text_for_type_selection = R"(
    *************************
    * TYPE OF THE HEAP ?    *
    *  0: max               *
    *  1: min               *
    *************************
    
    your choice:
        )";

        static bool is_valid_choice(int choice)
        {
            return choice >= 1 and choice <= 6;
        }
        static bool is_valid_type(int type)
        {
            return type == MIN_HEAP or type == MAX_HEAP;
        }

        public:
        int get_selection()
        {
            
            int selection = 0;


            /*
                we put a loop here to check while the user input is not valid .
                escape when we got the right input
            */
            while(true)
            {
                selection = display_some_things(option_text , true);
            

                if(is_valid_choice(selection)) break;

                display_some_things("INVALID CHOICE" , false);

            }

            return selection;

        }
        int get_type()
        {
            int heap_type = 0;
            while(true)
            {
                system("cls");
                heap_type = display_some_things(text_for_type_selection , true);
            

                if( is_valid_type(heap_type) ) break;

                display_some_things("INVALID CHOICE" , false);
            }

            return heap_type;
        }
        void display_greeting_and_instrucion()
        {
            display_some_things(greeting_text , false);

            display_some_things(instruction , false);

            
        }
        
        void insert_a_value(heap* Heap)
        {
            int value = display_some_things("VALUE to INSERT: " , true);

            Heap->insert(value);
        }
        void find_a_value(heap* Heap)
        {
            int value = display_some_things("VALUE to FIND: " , true);

            Heap->find(value);
        }
        void erase_a_value(heap* Heap)
        {
            int value = display_some_things("VALUE to ERASE: " , true);
            Heap->erase(value);
        }
};  




int main()
{
    MENU menu;

    menu.display_greeting_and_instrucion();

    //intiallize the heap type
    int type = menu.get_type();
    cout << "YOUR HEAP WILL BE " ;
    (type == MIN_HEAP) ? cout << "MIN HEAP" : cout << "MAX HEAP";

    heap* HEAP = new heap(type);

    while(true)
    {   
        int choice = menu.get_selection();

        switch(choice)
        {
            case 1:
                menu.insert_a_value(HEAP);
                break;
            case 2:
                menu.find_a_value(HEAP);
                break;
            case 3:
                cout <<"Top of the heap is: "<< HEAP->top();
                sleep(3);
                break;
            case 4:
                menu.erase_a_value(HEAP);
                break;
            case 5:
                HEAP->print_heap();
                break;
            case 6:
                return 0;
                break;

        }

    }

    // heap HEAP;

    // HEAP.insert(10);
    // HEAP.insert(11);
    // HEAP.print_heap();

    // // gotoxy(0 , 0);
    // // cout << "hello";
    // // gotoxy(3, 1);
    // // cout << "world";
}