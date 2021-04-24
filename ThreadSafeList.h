#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <pthread.h>
#include <iostream>
#include <iomanip> // std::setw

using namespace std;

template <typename T>
class List 
{
    public:
        /**
         * Constructor
         */
        List() : size(0) { 
		//TODO: add your implementation 
			pthread_mutex_init(&size_lock, NULL);
			pthread_mutex_init(&list_mutex, NULL);
			head = new Node();
		}

        /**
         * Destructor
         */
        ~List() {
			Node* temp = head->next;
			for (int i = 0; i < size; i++) {
				Node* to_delete = temp;
				temp = temp->next;
				delete to_delete;
			}
			delete head;
			pthread_mutex_destroy(&size_lock);
			pthread_mutex_destroy(&list_mutex);
		}

        class Node {
         public:
          T data;
          Node *next;
		  pthread_mutex_t node_lock;
		  
		  Node() : data(), next(NULL) {
			  pthread_mutex_init(&node_lock, NULL);
		  }
		  
		  explicit Node(T data_t) : data(data_t), next(NULL) {
			  pthread_mutex_init(&node_lock, NULL);
		  }
		  
		  ~Node() {
			  pthread_mutex_destroy(&node_lock);
		  }
        };

        /**
         * Insert new node to list while keeping the list ordered in an ascending order
         * If there is already a node has the same data as @param data then return false (without adding it again)
         * @param data the new data to be added to the list
         * @return true if a new node was added and false otherwise
         */
        bool insert(const T& data) {
			//TODO: add your implementation
			if (find(data))
				return false;
			
			Node* new_node = new Node(data);
			Node* curr = head;
			pthread_mutex_lock(&(curr->node_lock));
			Node* next = head->next;
			if (next)
				pthread_mutex_lock(&(next->node_lock));
			
			while (next) {
				if (next->data > new_node->data)
					break;
				
				pthread_mutex_unlock(&(curr->node_lock));
				curr = curr->next;
				next = next->next;
				if (next)
					pthread_mutex_lock(&(next->node_lock));
			}
			
			new_node->next = curr->next;
			curr->next = new_node;
			__add_hook();
			pthread_mutex_unlock(&(curr->node_lock));
			if (next)
				pthread_mutex_unlock(&(next->node_lock));
			
			pthread_mutex_lock(&size_lock);
			size++;
			pthread_mutex_unlock(&size_lock);
			return true;
        }

        /**
         * Remove the node that its data equals to @param value
         * @param value the data to lookup a node that has the same data to be removed
         * @return true if a matched node was found and removed and false otherwise
         */
        bool remove(const T& value) {
			//TODO: add your implementation
			if (!find(value))
				return false;
			
			Node* curr = head;
			pthread_mutex_lock(&(curr->node_lock));
			Node* next = head->next;
			pthread_mutex_lock(&(next->node_lock));
			
			while (next) {
				if (next->data == value)
					break;
				
				pthread_mutex_unlock(&(curr->node_lock));
				curr = curr->next;
				next = next->next;
				if (next)
					pthread_mutex_lock(&(next->node_lock));
			}
			
			Node* to_delete = next;
			curr->next = to_delete->next;
			__remove_hook();
			pthread_mutex_unlock(&(curr->node_lock));
			pthread_mutex_unlock(&(next->node_lock));
			delete to_delete;
			
			pthread_mutex_lock(&size_lock);
			size--;
			pthread_mutex_unlock(&size_lock);
			return true;
        }

        /**
         * Returns the current size of the list
         * @return the list size
         */
        unsigned int getSize() {
			//TODO: add your implementation
			pthread_mutex_lock(&size_lock);
			unsigned int res = size;
			pthread_mutex_unlock(&size_lock);
			return res;
        }

		// Don't remove
        void print() {
          pthread_mutex_lock(&list_mutex);
          Node* temp = head;
		  temp = head->next;
          if (temp == NULL)
          {
            cout << "";
          }
          else if (temp->next == NULL)
          {
            cout << temp->data;
          }
          else
          {
            while (temp != NULL)
            {
              cout << right << setw(3) << temp->data;
              temp = temp->next;
              cout << " ";
            }
          }
          cout << endl;
          pthread_mutex_unlock(&list_mutex);
        }

		// Don't remove
        virtual void __add_hook() {}
		// Don't remove
        virtual void __remove_hook() {}

    private:
	// TODO: Add your own methods and data members
        Node* head;
		int size;
		pthread_mutex_t size_lock;
		pthread_mutex_t list_mutex;
		
		bool find(T data) {
			Node* curr = head;
			pthread_mutex_lock(&(curr->node_lock));
			Node* next = head->next;
			if (next)
				pthread_mutex_lock(&(next->node_lock));
			while (next) {
				if (next->data == data) {
					pthread_mutex_unlock(&(curr->node_lock));
					pthread_mutex_unlock(&(next->node_lock));
					return true;
				}
								
				pthread_mutex_unlock(&(curr->node_lock));
				curr = curr->next;
				next = next->next;
				if (next)
					pthread_mutex_lock(&(next->node_lock));
			}
			pthread_mutex_unlock(&(curr->node_lock));
			if (next)
				pthread_mutex_unlock(&(next->node_lock));
			return false;
		}
};

#endif //THREAD_SAFE_LIST_H_