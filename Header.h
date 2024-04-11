#pragma once

#include "iostream"
#include "string"

using namespace std;

struct AssigneeNode
{
	int ID;

	string fName;
	string lName;
	string DOB;
	string address;
	string color;
	
	AssigneeNode* right;
	AssigneeNode* left;
	AssigneeNode* parent;

	AssigneeNode(string f, string l, string dob, string a, int id = -1, string c = "red")
	{
		fName = f;
		lName = l;
		DOB = dob;
		address = a;
		
		ID = id;
		color = c;
		right = left = parent = nullptr;
	}
};

struct TaskNode
{
	bool complete;

	int taskID;
	int priorityLevel;
	int assigneeID;

	string description;
	string color;
	
	TaskNode* right;
	TaskNode* left;
	TaskNode* parent;

	TaskNode(string d, int p, int tID, int aID, string c = "red")
	{
		description = d;
		priorityLevel = p;
		taskID = tID;
		assigneeID = aID;
		color = c;
		complete = false;

		right = left = parent = nullptr;
	}
};

class taskManagementSystem
{
public:
	AssigneeNode* assigneeRoot;
	TaskNode* taskRoot;
	int latestAssigneeID;

	taskManagementSystem()
	{
		latestAssigneeID = 0;
		assigneeRoot = nullptr;
		taskRoot = nullptr;
	}

	
	// ROTATIONS
	AssigneeNode* aLL(AssigneeNode* n1)
	{

		AssigneeNode* n2 = n1->right;
		n1->right = n2->left;

		if (n1->right != nullptr)
			n1->right->parent = n1;
	
		n2->left = n1;
		n2->parent = n1->parent;

		if (n1->parent != nullptr)
			n1->parent->right = n2;

		n1->parent = n2;
		
		if (assigneeRoot == n1)
			assigneeRoot = n2;

		return n2;
	}

	AssigneeNode* aRR(AssigneeNode* n1)
	{
		AssigneeNode* n2 = n1->left;
		n1->left = n2->right;

		if (n1->left != nullptr)
			n1->left->parent = n1;
		
		n2->right = n1;
		n2->parent = n1->parent;

		if (n1->parent != nullptr)
			n1->parent->left = n2;

		n1->parent = n2;

		if (assigneeRoot == n1)
			assigneeRoot = n2;

		return n2;
	}

	TaskNode* tLL(TaskNode* n1)
	{
		TaskNode* n2 = n1->right;
		n1->right = n2->left;
		n2->left = n1;
		n2->parent = n1->parent;
		n1->parent = n2;

		// n1 is Right Child
		if (n2->parent != nullptr && n2->parent->right == n1)
			n2->parent->right = n2;

		else if (n2->parent != nullptr && n2->parent->left == n1)
			n2->parent->left = n2;

		if (taskRoot == n1)
			taskRoot = n2;

		return n2;
	}

	TaskNode* tRR(TaskNode* n1)
	{
		TaskNode* n2 = n1->left;
		n1->left = n2->right;
		n2->right = n1;
		n2->parent = n1->parent;
		n1->parent = n2;

		// n1 is Right Child
		if (n2->parent != nullptr && n2->parent->right == n1)
			n2->parent->right = n2;

		else if (n2->parent != nullptr && n2->parent->left == n1)
			n2->parent->left = n2;

		if (taskRoot == n1)
			taskRoot = n2;

		return n2;
	}

	template <typename T>
	T* LR(T* n1)
	{
		n1->right = RR(n1->right);
		return LL(n1);
	}

	template <typename T>
	T* RL(T* n1)
	{
		n1->right = RR(n1->right);
		return LL(n1);
	}


	// INSERTIONS
	AssigneeNode* addAssigneeHELPER(AssigneeNode* p, AssigneeNode* n, bool rChild, string fName, string lName, string address, string DOB, int ID, bool& fixedImbalance)
	{
		// If location to insert the Node has been found
		if (n == nullptr)
		{
			n = new AssigneeNode(fName, lName, DOB, address, ID);
			n->parent = p;

			if (p != nullptr)
				if (rChild)
					p->right = n;
				else
					p->left = n;
		}

		// Can remove this
		else if (ID < n->ID)
			addAssigneeHELPER(n, n->left, false, fName, lName, address, DOB, ID, fixedImbalance);

		else if (ID > n->ID)
			addAssigneeHELPER(n, n->right, true, fName, lName, address, DOB, ID, fixedImbalance);

		// Fixing Imbalances only if Parent and GrandParent Exist
		if (n->parent != nullptr && n->parent->parent != nullptr && fixedImbalance == false)
			fixAssigneeImbalances(n, fixedImbalance);
		
		return n;
	}

	void addAssignee(string fName, string lName, string address, string DOB)
	{
		// assign appropriate ID to new Assignee
		latestAssigneeID++;
		bool fixedImbalance = false;

		if (assigneeRoot == nullptr)
			assigneeRoot = addAssigneeHELPER(nullptr, assigneeRoot, false, fName, lName, address, DOB, latestAssigneeID, fixedImbalance);
		else
			addAssigneeHELPER(nullptr, assigneeRoot, false, fName, lName, address, DOB, latestAssigneeID, fixedImbalance);

		
	}

	TaskNode* addTaskHELPER(TaskNode* p, TaskNode* n, bool rChild, int taskID, string description, int priorityLevel, int assigneeID, bool& fixedImbalance)
	{
		// If location to insert the Node has been found
		if (n == nullptr)
		{
			n = new TaskNode(description, priorityLevel, taskID, assigneeID);
			n->parent = p;

			if (p != nullptr)
				if (rChild)
					p->right = n;
				else
					p->left = n;
		}

		else if (priorityLevel < n->priorityLevel)
			addTaskHELPER(n, n->left, false, taskID, description, priorityLevel, assigneeID, fixedImbalance);

		else if (priorityLevel >= n->priorityLevel)
			addTaskHELPER(n, n->right, true, taskID, description, priorityLevel, assigneeID, fixedImbalance);

		// Fixing Imbalances only if Parent and GrandParent Exist
		if (n->parent != nullptr && n->parent->parent != nullptr && fixedImbalance == false)
			fixTaskImbalances(n, fixedImbalance);

		return n;
	}

	void addTask(int ID, string description, int priorityLevel, string assigneeID)
	{
		int id = (assigneeID[1] - '0') * 100 + (assigneeID[2] - '0') * 10 + (assigneeID[3] - '0');
		bool fixedImbalance = false;

		if (taskRoot == nullptr)
			taskRoot = addTaskHELPER(nullptr, taskRoot, false, ID, description, priorityLevel, id, fixedImbalance);
		else
			addTaskHELPER(nullptr, taskRoot, false, ID, description, priorityLevel, id, fixedImbalance);
	}


	// DELETIONS
	void assigneeTransplant(AssigneeNode* u, AssigneeNode* v)
	{
		if (u->parent == nullptr)
			assigneeRoot = v;
		else if (u->parent->left == u)
			u->parent->left = v;
		else
			u->parent->right = v;

		if (v != nullptr)
			v->parent = u->parent;
	}

	void taskTransplant(TaskNode* u, TaskNode* v)
	{
		if (u->parent == nullptr)
			taskRoot = v;
		else if (u->parent->left == u)
			u->parent->left = v;
		else
			u->parent->right = v;

		if (v != nullptr)
			v->parent = u->parent;
	}

	void deleteTempAssigneeNodes(AssigneeNode* n)
	{
		if (n != nullptr)
		{
			deleteTempAssigneeNodes(n->left);
			deleteTempAssigneeNodes(n->right);

			// If ID == -1, Delete it
			if (n->ID == -1)
			{
				if (n->parent->left == n)
					n->parent->left = nullptr;

				else if (n->parent->right == n)
					n->parent->right = nullptr;

				delete n;
				n = nullptr;
			}
		}
	}

	void deleteTempTaskNodes(TaskNode* n)
	{
		if (n != nullptr)
		{
			deleteTempTaskNodes(n->left);
			deleteTempTaskNodes(n->right);

			// If priority == -1, Delete it
			if (n->priorityLevel == -1)
			{
				if (n->parent->left == n)
					n->parent->left = nullptr;

				else if (n->parent->right == n)
					n->parent->right = nullptr;

				delete n;
				n = nullptr;
			}
		}
	}

	void deleteAssigneeFix(AssigneeNode* x)
	{
		AssigneeNode* w;

		while (x != assigneeRoot && x->color == "black")
		{
			//  x is a Left Child
			if (x->parent->left == x)
			{
				// w = x.sibling
				w = x->parent->right;

				if (w == nullptr)
				{
					w = new AssigneeNode("^", "^", "^", "^", -1, "black");
					x->parent->right = w;
				}

				w->parent = x->parent;

				if (w->right == nullptr)
				{
					w->right = new AssigneeNode("^", "^", "^", "^", -1, "black");
					w->right->parent = w;
				}

				if (w->left == nullptr)
				{
					w->left = new AssigneeNode("^", "^", "^", "^", -1, "black");
					w->left->parent = w;
				}

				// Case 1 : w is Red
				if (w->color == "red")
				{
					w->color = "black";
					x->parent->color = "red";

					if (x->ID == -1)
					{
						aLL(x->parent);
						w = x->parent->right;
					}

					else if (x->parent != nullptr && x->parent->parent != nullptr && (x->parent->parent->color == "red" || x->color == "red"))
					{
						aLL(x->parent);
						w = x->parent->right;
					}

					else
						break;
				}

				// Case 2 : w.right & w.left are both Black
				else if (w->right->color == "black" && w->left->color == "black")
				{
					w->color = "red";

					/*
					if (x->color == "red")
						w->color = "red";

					else if (x->color == "black" && x->parent->color == "black" && x->parent->parent != nullptr && x->parent->parent->color == "red")
						w->color = "red";

					else
						w->color = "black";
						*/
					
					x = x->parent;
				}

				// Case : w.right is Black
				else 
				{
					if (w->right->color == "black")
					{
						w->left->color = "black";
						w->color = "red";
						aRR(w);
						x = x->parent->right;
					}

					// Case 4 : w.left is Black
					w->color = x->parent->color;
					x->parent->color = "black";

					if (w->color == "black")
						w->right->color = "red";
					else
						w->right->color = "black";

					// or can do w.right.color = x.parent.color
					aLL(x->parent);
					x = assigneeRoot;
				}
			}

			// x is a Right Child
			else
			{
				// w = x.sibling
				w = x->parent->left;

				if (w == nullptr)
				{
					w = new AssigneeNode("^", "^", "^", "^", -1, "black");
					x->parent->left = w;
				}

				w->parent = x->parent;

				if (w->left == nullptr)
				{
					w->left = new AssigneeNode("^", "^", "^", "^", -1, "black");
					w->left->parent = w;
				}

				if (w->right == nullptr)
				{
					w->right = new AssigneeNode("^", "^", "^", "^", -1, "black");
					w->right->parent = w;
				}

				// Case 1 : w is Red
				if (w->color == "red")
				{
					w->color = "black";
					x->parent->color = "red";
					
					if (x->ID == -1)
					{
						aRR(x->parent);
						w = x->parent->left;
					}

					else if (x->parent->parent->color == "red" || x->color == "red")
					{
						aRR(x->parent);
						w = x->parent->left;
					}

					else
						break;
				}

				// Case 2 : w.right & w.left are both Black
				else if (w->right->color == "black" && w->left->color == "black")
				{
					w->color = "red";

					/*
					if (x->color == "red")
						w->color = "red";

					else if (x->color == "black" && x->parent->color == "black")
						w->color = "red";

					else
						w->color = "black";
						*/

					x = x->parent;
				}

				// Case : w.left is Black
				else
				{
					if (w->left->color == "black")
					{
						w->right->color = "black";
						w->color = "red";
						
						// Left Rotate or Right Rotate?
						aRR(w);
						x = x->parent->left;
					}

					// Case 4 : w.right is Black
					w->color = x->parent->color;
					x->parent->color = "black";
					
					if (w->color == "black")
						w->left->color = "red";
					else
						w->left->color = "black";

					// or can do w.right.color = x.parent.color
					aRR(x->parent);
					x = assigneeRoot;
				}
			}
		}

		x->color = "black";
	}

	void deleteTaskFix(TaskNode* x)
	{
		TaskNode* w;

		while (x != taskRoot && x->color == "black")
		{
			//  x is a Left Child
			if (x->parent->left == x)
			{
				// w = x.sibling
				w = x->parent->right;

				if (w == nullptr)
				{
					w = new TaskNode("^", -1, -1, -1, "black");
					x->parent->right = w;
				}

				w->parent = x->parent;

				if (w->right == nullptr)
				{
					w->right = new TaskNode("^", -1, -1, -1, "black");
					w->right->parent = w;
				}

				if (w->left == nullptr)
				{
					w->left = new TaskNode("^", -1, -1, -1, "black");
					w->left->parent = w;
				}

				// Case 1 : w is Red
				if (w->color == "red")
				{
					w->color = "black";
					x->parent->color = "red";

					if (x->priorityLevel == -1)
					{
						tLL(x->parent);
						w = x->parent->right;
					}

					else if (x->parent != nullptr && x->parent->parent != nullptr && (x->parent->parent->color == "red" || x->color == "red"))
					{
						tLL(x->parent);
						w = x->parent->right;
					}

					else
					{
						taskRoot->color = "black";
						break;
					}
				}

				// Case 2 : w.right & w.left are both Black
				else if (w->right->color == "black" && w->left->color == "black")
				{
					w->color = "red";

					/*
					if (x->color == "red")
						w->color = "red";

					else if (x->color == "black" && x->parent->color == "black" && x->parent->parent != nullptr && x->parent->parent->color == "red")
						w->color = "red";

					else
						w->color = "black";
						*/

					x = x->parent;
				}

				// Case : w.right is Black
				else
				{
					if (w->right->color == "black")
					{
						w->left->color = "black";
						w->color = "red";
						tRR(w);
						x = x->parent->right;
					}

					// Case 4 : w.left is Black
					w->color = x->parent->color;
					x->parent->color = "black";

					if (w->color == "black")
						w->right->color = "red";
					else
						w->right->color = "black";

					// or can do w.right.color = x.parent.color
					tLL(x->parent);
					x = taskRoot;
				}
			}

			// x is a Right Child
			else
			{
				// w = x.sibling
				w = x->parent->left;

				if (w == nullptr)
				{
					w = new TaskNode("^", -1, -1, -1, "black");
					x->parent->left = w;
				}

				w->parent = x->parent;

				if (w->left == nullptr)
				{
					w->left = new TaskNode("^", -1, -1, -1, "black");
					w->left->parent = w;
				}

				if (w->right == nullptr)
				{
					w->right = new TaskNode("^", -1, -1, -1, "black");
					w->right->parent = w;
				}

				// Case 1 : w is Red
				if (w->color == "red")
				{
					w->color = "black";
					x->parent->color = "red";

					if (x->priorityLevel == -1)
					{
						tRR(x->parent);
						w = x->parent->left;
					}

					else if (x->parent->parent->color == "red" || x->color == "red")
					{
						tRR(x->parent);
						w = x->parent->left;
					}

					else
					{
						taskRoot->color = "black";
						break;
					}
				}

				// Case 2 : w.right & w.left are both Black
				else if (w->right->color == "black" && w->left->color == "black")
				{
					w->color = "red";

					/*
					if (x->color == "red")
						w->color = "red";

					else if (x->color == "black" && x->parent->color == "black")
						w->color = "red";

					else
						w->color = "black";
						*/

					x = x->parent;
				}

				// Case : w.left is Black
				else
				{
					if (w->left->color == "black")
					{
						w->right->color = "black";
						w->color = "red";

						// Left Rotate or Right Rotate?
						tRR(w);
						x = x->parent->left;
					}

					// Case 4 : w.right is Black
					w->color = x->parent->color;
					x->parent->color = "black";

					if (w->color == "black")
						w->left->color = "red";
					else
						w->left->color = "black";

					// or can do w.right.color = x.parent.color
					tRR(x->parent);
					x = taskRoot;
				}
			}
		}

		x->color = "black";
	}

	void DeleteAssigneeHELPER(AssigneeNode* nodeToBeDeleted)
	{
		string orignalColor = nodeToBeDeleted->color;
		AssigneeNode* x;

		// Leaf Node
		if (nodeToBeDeleted->left == nullptr && nodeToBeDeleted->right == nullptr)
		{
			if (nodeToBeDeleted->color == "red")
			{
				if (nodeToBeDeleted->parent->left == nodeToBeDeleted)
					nodeToBeDeleted->parent->left = nullptr;

				if (nodeToBeDeleted->parent->right == nodeToBeDeleted)
					nodeToBeDeleted->parent->right = nullptr;

				nodeToBeDeleted = nullptr;
			}

			else
			{
				AssigneeNode* y = nodeToBeDeleted->parent;

				if (y->left == nodeToBeDeleted)
				{
					delete nodeToBeDeleted;
					nodeToBeDeleted = new AssigneeNode("^", "^", "^", "^", -1, "black");
					nodeToBeDeleted->left = nodeToBeDeleted->right = nullptr;
					y->left = nodeToBeDeleted;
					nodeToBeDeleted->parent = y;

					aLL(y->parent);

					if (nodeToBeDeleted->parent->color == "black")
					{
						y->parent->color = "black";
						y->color = "black";

						if (y->right != nullptr)
							y->right->color = "red";
					}

					else
					{
						y->parent->color = "red";
						y->color = "black";

						if (y->right != nullptr)
							y->right->color = "red";

						if (y->parent->right != nullptr)
							y->parent->right->color = "black";
					}

					delete nodeToBeDeleted;
					nodeToBeDeleted = nullptr;
					y->left = nullptr;
				}

				else
				{
					delete nodeToBeDeleted;
					nodeToBeDeleted = new AssigneeNode("^", "^", "^", "^", -1, "black");
					nodeToBeDeleted->left = nodeToBeDeleted->right = nullptr;
					y->right = nodeToBeDeleted;
					nodeToBeDeleted->parent = y;

					if (y->left->right != nullptr)
						aRR(y->left);
					
					y->parent->color = "black";
					y->color = "black";
					y->left->color = "red";
				}

				deleteAssigneeFix(y);
				deleteTempAssigneeNodes(assigneeRoot);
				return;
			}
		}

		// Case 1 : Left Child is nullptr
		else if (nodeToBeDeleted->left == nullptr)
		{
			x = nodeToBeDeleted->right;
			nodeToBeDeleted->right = x;

			if (x == nullptr)
				x = new AssigneeNode("^", "^", "^", "^", -1, "black");

			x->parent = nodeToBeDeleted;
			assigneeTransplant(nodeToBeDeleted, x);
		}

		// Case 2 : Right Child is nullptr
		else if (nodeToBeDeleted->right == nullptr)
		{
			x = nodeToBeDeleted->left;
			nodeToBeDeleted->left = x;

			if (x == nullptr)
				x = new AssigneeNode("^", "^", "^", "^", -1, "black");

			x->parent = nodeToBeDeleted;
			assigneeTransplant(nodeToBeDeleted, x);
		}

		else
		{
			// Find Maximum in the Left Subtree of z
			AssigneeNode* maxNode = nodeToBeDeleted->left;

			while (maxNode->right != nullptr)
				maxNode = maxNode->right;

			// Store maxNode.color in orignalColor
			orignalColor = maxNode->color;
			
			// Assign x to Left Child of maxNode
			x = maxNode->left;

			if (x == nullptr)
			{
				x = new AssigneeNode("^", "^", "^", "^", -1, "black");
				maxNode->left = x;
			}

			// If maxNode is a child of nodeToBeDeleted, then set the parent of x as maxNode
			x->parent = maxNode;
			
			if (nodeToBeDeleted->left != maxNode)
			{
				assigneeTransplant(maxNode, maxNode->right);
				maxNode->right = nodeToBeDeleted->right;
				maxNode->right->parent = maxNode;
			}

			assigneeTransplant(nodeToBeDeleted, maxNode);
			maxNode->right = nodeToBeDeleted->right;
			maxNode->right->parent = maxNode;
			maxNode->color = nodeToBeDeleted->color;

			if (maxNode->left != x)
			{
				maxNode->left = nodeToBeDeleted->left;
				maxNode->left->parent = maxNode;
				maxNode->left->right = x;
				x->parent = maxNode->left;
			}
		}

		delete nodeToBeDeleted;
		nodeToBeDeleted = nullptr;
	
		if (orignalColor == "black")
			deleteAssigneeFix(x);

		assigneeRoot->color = "black";
		deleteTempAssigneeNodes(assigneeRoot);
	}

	void DeleteAssignee(string ID)
	{
		int id = (ID[1] - '0')*100 + (ID[2] - '0')*10 + (ID[3] - '0');
		bool found1 = false;
		AssigneeNode* k = searchAssigneeInAssigneeTree(id, assigneeRoot, found1);
		bool found = false;
		TaskNode* temp = searchAssigneeInTaskTree(id, taskRoot, found);
		searchAssigneeInAssigneeTree(k->ID, assigneeRoot, found1);

		if (found1 == false || (temp != nullptr && temp->complete == false))
			cout << "\nCannot delete Assignee at the moment.\nEither the Assignee does not exist or has not completed their tasks.\n\n";
		else
			DeleteAssigneeHELPER(k);
	}

	void deleteTaskHELPER(TaskNode* nodeToBeDeleted)
	{
		string orignalColor = nodeToBeDeleted->color;
		TaskNode* x;

		// Leaf Node
		if (nodeToBeDeleted->left == nullptr && nodeToBeDeleted->right == nullptr)
		{
			if (nodeToBeDeleted->color == "red")
			{
				if (nodeToBeDeleted->parent->left == nodeToBeDeleted)
					nodeToBeDeleted->parent->left = nullptr;

				if (nodeToBeDeleted->parent->right == nodeToBeDeleted)
					nodeToBeDeleted->parent->right = nullptr;

				nodeToBeDeleted = nullptr;
			}

			else
			{
				TaskNode* y = nodeToBeDeleted->parent;

				if (y->left == nodeToBeDeleted)
				{
					delete nodeToBeDeleted;
					nodeToBeDeleted = new TaskNode("^", -1, -1, -1, "black");
					nodeToBeDeleted->left = nodeToBeDeleted->right = nullptr;
					y->left = nodeToBeDeleted;
					nodeToBeDeleted->parent = y;

					tLL(y->parent);

					if (nodeToBeDeleted->parent->color == "black")
					{
						y->parent->color = "black";
						y->color = "black";

						if (y->right != nullptr)
							y->right->color = "red";
					}

					else
					{
						y->parent->color = "red";
						y->color = "black";

						if (y->right != nullptr)
							y->right->color = "red";

						if (y->parent->right != nullptr)
							y->parent->right->color = "black";
					}

					delete nodeToBeDeleted;
					nodeToBeDeleted = nullptr;
					y->left = nullptr;
				}

				else
				{
					delete nodeToBeDeleted;
					nodeToBeDeleted = new TaskNode("^", -1, -1, -1, "black");
					nodeToBeDeleted->left = nodeToBeDeleted->right = nullptr;
					y->right = nodeToBeDeleted;
					nodeToBeDeleted->parent = y;

					if (y->left->right != nullptr)
						tRR(y->left);

					y->parent->color = "black";
					y->color = "black";
					y->left->color = "red";
				}

				deleteTaskFix(y);
				deleteTempTaskNodes(taskRoot);
				return;
			}
		}

		// Case 1 : Left Child is nullptr
		else if (nodeToBeDeleted->left == nullptr)
		{
			x = nodeToBeDeleted->right;
			nodeToBeDeleted->right = x;

			if (x == nullptr)
				x = new TaskNode("^", -1, -1, -1, "black");

			x->parent = nodeToBeDeleted;
			taskTransplant(nodeToBeDeleted, x);
		}

		// Case 2 : Right Child is nullptr
		else if (nodeToBeDeleted->right == nullptr)
		{
			x = nodeToBeDeleted->left;
			nodeToBeDeleted->left = x;

			if (x == nullptr)
				x = new TaskNode("^", -1, -1, -1, "black");

			x->parent = nodeToBeDeleted;
			taskTransplant(nodeToBeDeleted, x);
		}

		else
		{
			// Find Maximum in the Left Subtree of z
			TaskNode* maxNode = nodeToBeDeleted->left;

			while (maxNode->right != nullptr)
				maxNode = maxNode->right;

			// Store maxNode.color in orignalColor
			orignalColor = maxNode->color;

			// Assign x to Left Child of maxNode
			x = maxNode->left;

			if (x == nullptr && maxNode->color == "black")
			{
				x = new TaskNode("^", -1, -1, -1, "black");
				maxNode->left = x;
				x->parent = maxNode;
			}

			if (nodeToBeDeleted->left != maxNode)
			{
				taskTransplant(maxNode, maxNode->right);
				maxNode->left = nodeToBeDeleted->left;
				maxNode->left->parent = maxNode;
			}

			taskTransplant(nodeToBeDeleted, maxNode);
			maxNode->right = nodeToBeDeleted->right;
			maxNode->right->parent = maxNode;
			maxNode->color = nodeToBeDeleted->color;
			
			if (maxNode->left != x)
			{
				maxNode->left = nodeToBeDeleted->left;
				maxNode->left->parent = maxNode;

				if (maxNode->left->right == nullptr)
					maxNode->left->right = x;

				if (x != nullptr)
					x->parent = maxNode->left;
			}
		}

		delete nodeToBeDeleted;
		nodeToBeDeleted = nullptr;

		if (orignalColor == "black")
			deleteTaskFix(x);

		taskRoot->color = "black";
		deleteTempTaskNodes(taskRoot);
	}

	void deleteTask(int ID)
	{
		bool found = false;
		TaskNode* temp = searchTask(ID, taskRoot, found);

		if (found == false || temp == nullptr)
			cout << "\nCannot delete Task because it does not exist.\n\n";
		else
			deleteTaskHELPER(temp);
	}


	// Fixing Up imbalances
	void fixTaskImbalances(TaskNode* n, bool& fixedImbalance)
	{
		while (n->parent->color == "red" && n->color == "red")
		{
			// Parent is Right Child
			if (n->parent->parent->right == n->parent)
			{
				// Case 2 : n's Uncle is Red	// What if n's Uncle is Null? NILL is Black
				if (n->parent->parent->left != nullptr && n->parent->parent->left->color == "red")
				{
					// GrandParent
					n->parent->parent->color = "red";

					// Parent
					n->parent->color = "black";

					// Uncle
					n->parent->parent->left->color = "black";

					// Assign Grandparent to New Node?
					n = n->parent->parent;
				}

				// Case 3 : n's Uncle is Black (triangle)
				else
				{
					if (n->parent->left == n && (n->parent->parent->left == nullptr || n->parent->parent->left->color == "black"))
					{
						// Assign Parent to New Node?
						//n = n->parent;
						tRR(n->parent);
						tLL(n->parent);
						n->color = "black";
						n->left->color = "red";
					}

					else
					{
						// Case 4 : n's Uncle is Black (line)	// Check where to assign rotation
						tLL(n->parent->parent);

						n->parent->color = "black";
						n->parent->left->color = "red";
					}
				}
			}

			// Parent is Left Child
			else
			{
				// Case 2 : n's Uncle is Red
				if (n->parent->parent->right != nullptr && n->parent->parent->right->color == "red")
				{
					// GrandParent
					n->parent->parent->color = "red";

					// Parent
					n->parent->color = "black";

					// Uncle
					n->parent->parent->right->color = "black";

					n = n->parent;
				}

				// Case 3 : n's Uncle is Black (triangle)
				else
				{
					if (n->parent->right == n && (n->parent->parent->right == nullptr || n->parent->parent->right->color == "black"))
					{
						// Assign Parent to New Node?
						//n = n->parent;
						tLL(n->parent);
						tRR(n->parent);
						n->color = "black";
						n->left->color = "red";
					}

					else
					{
						// Case 4 : n's Uncle is Black (line)	// Check where to assign rotation
						tRR(n->parent->parent);
						
						n->parent->color = "black";
						n->parent->right->color = "red";
					}
				}
			}

			if (n == taskRoot)
				break;
		}

		// Root will always be Black
		taskRoot->color = "black";
		fixedImbalance = true;
	}

	void fixAssigneeImbalances(AssigneeNode* n, bool& fixedImbalance)
	{
		while (n->parent->color == "red" && n->color == "red")
		{
			// Parent is Right Child
			if (n->parent->parent->right == n->parent)
			{
				// Case 2 : n's Uncle is Red	// What if n's Uncle is Null? NILL is Black
				if (n->parent->parent->left != nullptr && n->parent->parent->left->color == "red")
				{
					// GrandParent
					n->parent->parent->color = "red";
				
					// Parent
					n->parent->color = "black";

					// Uncle
					n->parent->parent->left->color = "black";

					// Assign Grandparent to New Node?
					n = n->parent->parent;
				}

				// Case 3 : n's Uncle is Black (triangle)
				else 
				{
					if (n->parent->parent->left == n->parent && (n->parent->parent->left == nullptr || n->parent->parent->left->color == "black"))
					{
						// Assign Parent to New Node?
						n = n->parent;
						aRR(n);
					}

					// Case 4 : n's Uncle is Black (line)	// Check where to assign rotation

					// Parent
					n->parent->color = "black";

					// GrandParent
					n->parent->parent->color = "red";
					aLL(n->parent->parent);
				}	
			}

			// Parent is Left Child
			else
			{
				// Case 2 : n's Uncle is Red	// What if n's Uncle is Null? NILL is Black
				if (n->parent->parent->right != nullptr && n->parent->parent->right->color == "red")
				{
					// GrandParent
					n->parent->parent->color = "red";

					// Parent
					n->parent->color = "black";

					// Uncle
					n->parent->parent->right->color = "black";

					// Assign Grandparent to New Node?
					n = n->parent->parent;
				}

				// Case 3 : n's Uncle is Black (triangle)
				else
				{
					if (n->parent->parent->right == n->parent && (n->parent->parent->right == nullptr || n->parent->parent->right->color == "black"))
					{
						// Assign Parent to New Node?
						n = n->parent;
						aLL(n);
					}

					// Case 4 : n's Uncle is Black (line)	// Check where to assign rotation

					// Parent
					n->parent->color = "black";

					// GrandParent
					n->parent->parent->color = "red";
					aRR(n->parent->parent);
				}
			}

			// Break if n == Root to prevent exeption
			if (n == assigneeRoot)
				break;
		}

		// Root will always be Black
		assigneeRoot->color = "black";
		fixedImbalance = true;
	}


	// DISPLAY
	void displaybyID(stringstream& actualOutput, string ID)
	{
		AssigneeNode* temp = assigneeRoot;
		int idToBeSearched = (ID[1] - '0') + (ID[2] - '0') + (ID[3] - '0');
		
		while (temp->ID != idToBeSearched)
			if (idToBeSearched > temp->ID)
				temp = temp->right;
			else
				temp = temp->left;

		// Temp is now the Node to be Displayed
		actualOutput << "Name: " + temp->fName + " " + temp->lName + ", Address: " + temp->address + ", DOB: " + temp->DOB + ", Assignee ID: " + ID << endl;
	}

	void displaybyname(stringstream& actualOutput, string fName, string lName)
	{
		displayByNameHELPER(assigneeRoot, actualOutput, fName, lName);
	}
	
	void displayByNameHELPER(AssigneeNode* n, stringstream& actualOutput, string fName, string lName)
	{
		if (n == nullptr)
			return;

		displayByNameHELPER(n->left, actualOutput, fName, lName);

		if (n->fName == fName && n->lName == lName)
		{
			actualOutput << "Name: " + n->fName + " " + n->lName + ", Address: " + n->address + ", DOB: " + n->DOB + ", Assignee ID: A";

			if (n->ID <= 9)
				actualOutput << "00" << n->ID;
			else if (n->ID <= 99)
				actualOutput << "0" << n->ID;
			else
				actualOutput << n->ID;

			actualOutput << endl;
		}

		displayByNameHELPER(n->right, actualOutput, fName, lName);
	}

	void AssigneeWithNoTask(stringstream& actualOutput)
	{
		AssigneeWithNoTaskHELPER(actualOutput, assigneeRoot);
	}

	void AssigneeWithNoTaskHELPER(stringstream& actualOutput, AssigneeNode* assignee)
	{
		if (assignee == nullptr)
			return;

		AssigneeWithNoTaskHELPER(actualOutput, assignee->left);

		bool check = true;
		checkTasks(assignee->ID, taskRoot, check);

		// Assignee With No Tasks		
		if (check == true)
		{
			actualOutput << "Name: " + assignee->fName + " " + assignee->lName + ", Address: " + assignee->address + ", DOB: " + assignee->DOB + ", Assignee ID: A";
			
			if (assignee->ID <= 9)
				actualOutput << "00" << assignee->ID;
			else if (assignee->ID <= 99)
				actualOutput << "0" << assignee->ID;
			else
				actualOutput << assignee->ID;

			actualOutput << endl;
		}

		AssigneeWithNoTaskHELPER(actualOutput, assignee->right);
	}
	
	void printTaskQueue(stringstream& actualOutput)
	{
		inorderTaskTree(taskRoot, actualOutput);
	}

	void displayCompletedTasksHELPER(TaskNode* n, stringstream& actualOutput)
	{
		if (n != nullptr)
		{
			if (n->complete)
			{
				bool found = false;
				AssigneeNode* temp = searchAssigneeInAssigneeTree(n->assigneeID, assigneeRoot, found);
				actualOutput << "Task ID: " << n->taskID << ", Description: " << n->description << ", Priority: " << n->priorityLevel << ", Assignee: " << temp->fName << " (A";

				if (temp->ID <= 9)
					actualOutput << "00" << temp->ID;
				else if (temp->ID <= 99)
					actualOutput << "0" << temp->ID;
				else
					actualOutput << temp->ID;

				actualOutput << ")" << endl;
			}

			displayCompletedTasksHELPER(n->left, actualOutput);
			displayCompletedTasksHELPER(n->right, actualOutput);
		}
	}

	void displayCompletedTasks(stringstream& actualOutput)
	{
		actualOutput << "Completed Tasks:\n";
		displayCompletedTasksHELPER(taskRoot, actualOutput);
	}

	void displayAssigneeHELPER(stringstream& actualOutput, AssigneeNode* n)
	{
		if (n != nullptr)
		{
			displayAssigneeHELPER(actualOutput, n->left);
			actualOutput << "Name: " + n->fName + " " + n->lName + ", Address: " + n->address + ", DOB: " + n->DOB + ", Assignee ID: A";
			
			if (n->ID <= 9)
				actualOutput << "00" << n->ID;
			else if (n->ID <= 99)
				actualOutput << "0" << n->ID;
			else
				actualOutput << n->ID;

			actualOutput << endl;

			displayAssigneeHELPER(actualOutput, n->right);
		}
	}

	void DisplayAssignee(stringstream& actualOutput)
	{
		displayAssigneeHELPER(actualOutput, assigneeRoot);
	}

	void AssigneeInOrderHELPER(stringstream& actualOutput, AssigneeNode* n)
	{
		if (n != nullptr)
		{
			//cout << n->ID << " ";
			// Traverse Left
			AssigneeInOrderHELPER(actualOutput, n->left);
			
			// Add Assignee to Output
			actualOutput << "A";

			if (n->ID <= 9)
				actualOutput << "00" << n->ID;
			else if (n->ID <= 99)
				actualOutput << "0" << n->ID;
			else
				actualOutput << n->ID;

			actualOutput << " (" << n->color << ")\n";

			//cout << actualOutput.str() << endl << endl;

			// Traverse Right
			AssigneeInOrderHELPER(actualOutput, n->right);
		}
	}

	void AssigneeInOrder(stringstream& actualOutput)
	{
		AssigneeInOrderHELPER(actualOutput, assigneeRoot);
	}

	void inorderTaskTree(TaskNode* n, stringstream& actualOutput)
	{
		if (n != nullptr)
		{
			inorderTaskTree(n->left, actualOutput); 
			
			// Output Task Node
			bool found = false;
			AssigneeNode* temp = searchAssigneeInAssigneeTree(n->assigneeID, assigneeRoot, found);
			
			if (n->complete != true)
			{
				actualOutput << "Task ID: " << n->taskID << ", Description: " << n->description << ", Priority: " << n->priorityLevel << ", Assignee: " << temp->fName << " (A";

				if (temp->ID <= 9)
					actualOutput << "00" << temp->ID;
				else if (temp->ID <= 99)
					actualOutput << "0" << temp->ID;
				else
					actualOutput << temp->ID;

				actualOutput << ")" << endl;
			}

			inorderTaskTree(n->right, actualOutput);
		}
	}

	void inOrderAssigneeTree(AssigneeNode* n)
	{
		if (n != nullptr)
		{
			inOrderAssigneeTree(n->left);
			cout << n->ID << " ";
			inOrderAssigneeTree(n->right);
		}
	}

	void PrintTreeInorderHELPER(TaskNode* n, stringstream& actualOutput)
	{
		if (n != nullptr)
		{
			PrintTreeInorderHELPER(n->left, actualOutput);
			actualOutput << n->taskID << " (" << n->color << ")\n";
			PrintTreeInorderHELPER(n->right, actualOutput);
		}
	}

	void PrintTreeInorder(stringstream& actualOutput)
	{
		PrintTreeInorderHELPER(taskRoot, actualOutput);
	}


	// Check which Assignees have which Tasks
	void checkTasks(int assigneeID, TaskNode* n, bool & check)
	{
		if (n == nullptr)
			return;
		
		check = true;
		checkTasks(assigneeID, n->left, check);
		
		if (check == true)
			checkTasks(assigneeID, n->right, check);
		
		if (assigneeID == n->assigneeID)
		{
			check = false;
			return;
		}
	}

	void findTasksByAssignee(string ID, stringstream& actualOutput)
	{
		int id = (ID[1] - '0')*100 + (ID[2] - '0')*10 + (ID[3] - '0');
		bool found = false;
		AssigneeNode* a = searchAssigneeInAssigneeTree(id, assigneeRoot, found);

		actualOutput << "Tasks Assigned to \"" << a->fName << " (A";

		if (a->ID <= 9)
			actualOutput << "00" << a->ID;
		else if (a->ID <= 99)
			actualOutput << "0" << a->ID;
		else
			actualOutput << a->ID;

		actualOutput << ")\":" << endl;

		findTasksByAssigneeHELPER(taskRoot, id, actualOutput, a);
	}

	void findTasksByAssigneeHELPER(TaskNode* n, int ID, stringstream& actualOutput, AssigneeNode* a)
	{
		if (n != nullptr)
		{
			findTasksByAssigneeHELPER(n->left, ID, actualOutput, a);
			findTasksByAssigneeHELPER(n->right, ID, actualOutput, a);

			if (n->assigneeID == ID)
			{
				actualOutput << "Task ID: " << n->taskID << ", Description: " << n->description << ", Priority: " << n->priorityLevel << ", Assignee: " << a->fName << " (A";
				
				if (a->ID <= 9)
					actualOutput << "00" << a->ID;
				else if (a->ID <= 99)
					actualOutput << "0" << a->ID;
				else
					actualOutput << a->ID;

				actualOutput << ")" << endl;
			}
		}
	}

	
	// Search Assignees
	TaskNode* searchAssigneeInTaskTree(int assigneeID, TaskNode* n, bool& found)
	{
		if (n == nullptr)
			return nullptr;

		if (assigneeID == n->assigneeID)
		{
			found = true;
			return n;
		}

		TaskNode* temp = searchAssigneeInTaskTree(assigneeID, n->left, found);
		
		if (temp == nullptr && found == false)
			temp = searchAssigneeInTaskTree(assigneeID, n->right, found);

		return temp;
	}
	
	AssigneeNode* searchAssigneeInAssigneeTree(int assigneeID, AssigneeNode* n, bool& found)
	{
		if (n == nullptr)
			return nullptr;

		if (assigneeID == n->ID)
		{
			found = true;
			return n;
		}

		AssigneeNode* temp = searchAssigneeInAssigneeTree(assigneeID, n->left, found);

		if (temp == nullptr && found == false)
			temp = searchAssigneeInAssigneeTree(assigneeID, n->right, found);

		return temp;
	}


	// Search Tasks
	void findHighestPriorityTask(stringstream& actualOutput)
	{
		TaskNode* n = taskRoot;

		while (n->left != nullptr)
			n = n->left;
		
		bool found = false;
		AssigneeNode* temp = searchAssigneeInAssigneeTree(n->assigneeID, assigneeRoot, found);
		actualOutput << "Highest Priority Task: Task ID: " << n->taskID << ", Description: " << n->description << ", Priority: " << n->priorityLevel << ", Assignee: " << temp->fName << " (A";
		
		if (temp->ID <= 9)
			actualOutput << "00" << temp->ID;
		else if (temp->ID <= 99)
			actualOutput << "0" << temp->ID;
		else
			actualOutput << temp->ID;

		actualOutput << ")" << endl;

		// Check for more Tasks of the same priority
		while (n->parent != nullptr && n->parent->priorityLevel == n->priorityLevel)
		{
			n = n->parent;

			bool found = false;
			AssigneeNode* temp = searchAssigneeInAssigneeTree(n->assigneeID, assigneeRoot, found);
			actualOutput << "Highest Priority Task: Task ID: " << n->taskID << ", Description: " << n->description << ", Priority: " << n->priorityLevel << ", Assignee: " << temp->fName << " (A";

			if (temp->ID <= 9)
				actualOutput << "00" << temp->ID;
			else if (temp->ID <= 99)
				actualOutput << "0" << temp->ID;
			else
				actualOutput << temp->ID;

			actualOutput << ")" << endl;
		}
	}

	void searchTasksByPriorityRangeHELPER(int PL, TaskNode* n, stringstream& actualOutput)
	{
		if (n != nullptr)
		{
			if (n->priorityLevel == PL)
			{
				bool found = false;
				AssigneeNode* temp = searchAssigneeInAssigneeTree(n->assigneeID, assigneeRoot, found);
				actualOutput << "Task ID: " << n->taskID << ", Description: " << n->description << ", Priority: " << n->priorityLevel << ", Assignee: " << temp->fName << " (A";

				if (temp->ID <= 9)
					actualOutput << "00" << temp->ID;
				else if (temp->ID <= 99)
					actualOutput << "0" << temp->ID;
				else
					actualOutput << temp->ID;

				actualOutput << ")" << endl;
			}

			searchTasksByPriorityRangeHELPER(PL, n->left, actualOutput);
			searchTasksByPriorityRangeHELPER(PL, n->right, actualOutput);
		}
	}

	TaskNode* searchTask(int ID, TaskNode* n, bool& found)
	{
		if (n != nullptr)
		{
			if (n->taskID == ID)
			{
				found = true;
				return n;
			}

			TaskNode* temp = searchTask(ID, n->left, found);

			if (temp != nullptr)
				found = true;
			
			if (temp == nullptr && found == false)
				temp = searchTask(ID, n->right, found);
			
			if (found == true)
				return temp;
		}

		return nullptr;
	}

	void searchTasksByPriorityRange(int a, int b, stringstream& actualOutput)
	{
		actualOutput << "Tasks within Priority Range (" << a << " to " << b << "):\n";

		for (int i = a; i <= b; i++)
			searchTasksByPriorityRangeHELPER(i, taskRoot, actualOutput);
	}


	// Shift Task of one Assignee to another
	void ShiftTask(string ID1, string ID2)
	{
		int fromID = (ID1[1] - '0') + (ID1[2] - '0') + (ID1[3] - '0');
		int toID = (ID2[1] - '0') + (ID2[2] - '0') + (ID2[3] - '0');
		bool found = false;
		TaskNode* n1 = searchAssigneeInTaskTree(fromID, taskRoot, found);
		TaskNode* n2 = searchAssigneeInTaskTree(toID, taskRoot, found);

		// Shift Tasks
		if (n1 != nullptr && n2 != nullptr)
			n1->assigneeID = n2->assigneeID;
	}


	// Update Task Priority
	void updateTaskPriority(int ID, int priority)
	{
		bool found = false;
		TaskNode* temp = searchTask(ID, taskRoot, found);

		TaskNode* x = new TaskNode(temp->description, priority, temp->taskID, temp->assigneeID, temp->color);
		x->complete = temp->complete;

		deleteTask(ID);

		string str = "A";

		if (x->assigneeID <= 9)
			str += "00" + to_string(x->assigneeID);
		else if (ID <= 99)
			str += "0" + to_string(x->assigneeID);
		else
			str += to_string(x->assigneeID);
		
		addTask(x->taskID, x->description, x->priorityLevel, str);
	}


	// GENERATE IDs
	int generateAssigneeID()
	{
		if (assigneeRoot == nullptr)
			return 1;

		// Search for the Biggest ID
		else
		{
			AssigneeNode* temp = assigneeRoot;

			while (temp->right != nullptr)
				temp = temp->right;

			// Temp is now at Assignee with maximum ID
			return temp->ID + 1;
		}
	}


	// Task Completion
	void completeTask(int ID)
	{
		bool found = false;
		TaskNode* temp = searchTask(ID, taskRoot, found);
		temp->complete = true;
	}

	void clearCompletedTasksHELPER(TaskNode* n)
	{
		if (n != nullptr)
		{
			if (n->complete)
				deleteTask(n->taskID);

			clearCompletedTasksHELPER(n->left);
			clearCompletedTasksHELPER(n->right);
		}
	}

	void clearCompletedTasks()
	{
		clearCompletedTasksHELPER(taskRoot);
	}


	// Count Number of Assigned Tasks
	void countTotalTasks(stringstream& actualOutput)
	{
		actualOutput << "Total Tasks in the System: ";
		
		int count = 0;
		countTotalTasksHELPER(taskRoot, actualOutput, count);

		actualOutput << count << "\n";
	}

	void countTotalTasksHELPER(TaskNode* n, stringstream& actualOutput, int& count)
	{
		if (n != nullptr)
		{
			countTotalTasksHELPER(n->right, actualOutput, count);
			countTotalTasksHELPER(n->left, actualOutput, count);

			count++;
		}
	}
};