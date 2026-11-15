#include <iostream>
#include <cstring>
#include <iomanip>
using namespace std;

#define SIZE 5

struct Node
{
    char key[100];
    char value[100];
    int frequency;
    int lastUsed;
    Node *prev;
    Node *next;
};
struct Cache
{
    Node *head;
    Node *tail;
    int count;
    int hits;
    int misses;
};
Cache mainCache = {NULL, NULL, 0, 0, 0};
int timerCount = 0;
Node* search(char key[])
{
    Node *temp = mainCache.head;
    while (temp != NULL)
    {
        if (strcmp(temp->key, key) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

Node* createNode(char key[], char value[])
{
    Node *n = new Node;
    strcpy(n->key, key);
    strcpy(n->value, value);
    n->frequency = 1;
    n->lastUsed = timerCount;
    n->prev = NULL;
    n->next = NULL;
    return n;
}

void addFront(Node *n)
{
    n->prev = NULL;
    n->next = mainCache.head;
    if (mainCache.head != NULL)
        mainCache.head->prev = n;
    else
        mainCache.tail = n;
    mainCache.head = n;
    mainCache.count++;
}

void removeNode(Node *n)
{
    if (n->prev != NULL)
        n->prev->next = n->next;
    else
        mainCache.head = n->next;
    if (n->next != NULL)
        n->next->prev = n->prev;
    else
        mainCache.tail = n->prev;

    delete n;
    mainCache.count--;
}

Node* LRU()
{
    Node *temp = mainCache.head;
    Node *oldest = mainCache.head;
    while (temp != NULL)
    {
        if (temp->lastUsed < oldest->lastUsed)
            oldest = temp;
        temp = temp->next;
    }
    return oldest;
}

Node* LFU()
{
    Node *temp = mainCache.head;
    Node *least = mainCache.head;
    while (temp != NULL)
    {
        if (temp->frequency < least->frequency)
        {
            least = temp;
        }
        else if (temp->frequency == least->frequency)
        {
            if (temp->lastUsed < least->lastUsed)
                least = temp;
        }

        temp = temp->next;
    }

    return least;
}

void EVICT()
{
    if (mainCache.head == NULL)
        return;
    Node *lruNode = LRU();
    Node *lfuNode = LFU();
    cout << "\nLRU candidate: " << lruNode->key << endl;
    cout << "LFU candidate: " << lfuNode->key << endl;
    Node *victim = lfuNode;
    cout << "Evicted key: " << victim->key << endl;
    removeNode(victim);
}
void SET(char key[], char value[])
{
    timerCount++;
    Node *temp = search(key);
    if (temp != NULL)
    {
        strcpy(temp->value, value);
        temp->frequency++;
        temp->lastUsed = timerCount;
        cout << "Value updated successfully\n";
        return;
    }

    if (mainCache.count >= SIZE)
        EVICT();

    Node *n = createNode(key, value);
    addFront(n);

    cout << "Key inserted successfully\n";
}
void GET(char key[])
{
    timerCount++;

    Node *temp = search(key);

    if (temp == NULL)
    {
        mainCache.misses++;
        cout << "Key not found\n";
        return;
    }

    mainCache.hits++;
    temp->frequency++;
    temp->lastUsed = timerCount;

    cout << "Value: " << temp->value << endl;
}
void DELETE(char key[])
{
    Node *temp = search(key);

    if (temp == NULL)
    {
        cout << "Key not found\n";
        return;
    }
    removeNode(temp);

    cout << "Key deleted successfully\n";
}

void DISPLAY()
{
    Node *temp = mainCache.head;
    cout << "\nCache Contents:\n";
    if (temp == NULL)
    {
        cout << "Cache is empty\n";
        return;
    }

    while (temp != NULL)
    {
        cout << "Key: " << temp->key
             << " Value: " << temp->value
             << " Frequency: " << temp->frequency
             << " Last Used: " << temp->lastUsed
             << endl;

        temp = temp->next;
    }

    cout << "Total entries: " << mainCache.count << endl;
}

void STATS()
{
    cout << "\nCache Statistics:\n";
    cout << "Hits: " << mainCache.hits << endl;
    cout << "Misses: " << mainCache.misses << endl;
    cout << "Total entries: " << mainCache.count << endl;
}

int main()
{
    int choice;
    char key[100], value[100];
    do
    {
        cout << "\nKeyFlux Menu:\n";
        cout << "1. SET\n";
        cout << "2. GET\n";
        cout << "3. DELETE\n";
        cout << "4. DISPLAY\n";
        cout << "5. STATISTICS\n";
        cout << "6. EXIT\n";
        cout << "Enter choice: ";
        cin >> choice;
        switch (choice)
        {
            case 1:
                cout << "Enter key: ";
                cin >> setw(100) >> key;

                cout << "Enter value: ";
                cin >> setw(100) >> value;

                SET(key, value);
                break;
            case 2:
                cout << "Enter key: ";
                cin >> setw(100) >> key;
                GET(key);
                break;
            case 3:
                cout << "Enter key: ";
                cin >> setw(100) >> key;
                DELETE(key);
                break;
            case 4:
                DISPLAY();
                break;
            case 5:
                STATS();
                break;
            case 6:
                cout << "Exiting KeyFlux\n";
                break;

            default:
                cout << "Invalid choice\n";
        }

    } while (choice != 6);

    while (mainCache.head != NULL)
        removeNode(mainCache.head);
    return 0;
}
