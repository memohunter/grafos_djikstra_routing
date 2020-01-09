

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>



typedef int bool; 
enum {false,
      true};

typedef struct    
{
    char name[2]; 
    int cost_from_start;
    bool is_start;
    bool is_end;
    bool visited;
    char prev_name[2];
    char neighbors[100][2]; 
    int num_neighbors;
} Node;

typedef struct   
{
    char name1[2];
    char name2[2];
    int cost_of_edge;
} Edge;

bool exister(Node* arr, int arr_size, char target[2]); 
void node_setter(Node* n, char name[2], bool start, bool end, bool visited, int inital_cost, char prev[2]);
void edge_setter(Edge* e, char name1[2], char name2[2], int edge_cost);

int main()
{
    char choice_start[2];
    char choice_end[2];
    int num_visited = 0;
    int num_nodes = 0;


    Node nodes[100];
    Node last_obj;
    int start_index = -2;  
    int last_obj_index = -2;
    Edge edges[500];
    Node* current; 

   
    printf("Por favor ingresa el nodo Inicial: ");
    scanf("%s", choice_start);

    printf("Por favor ingresa el nodo Final: ");
    scanf("%s", choice_end);

    FILE* fpointer = fopen("djikstra_Routing.txt","r"); 
    int num_lines = 0; 

    rewind(fpointer);


    while(!feof(fpointer))
    {
        bool name1_exists = false;
        bool name2_exists = false;
        char n1[2];
        char n2[2];

        int edge_cost;
        fscanf(fpointer, "%s %s %d", n1, n2, &edge_cost);

        
        edge_setter(&edges[num_lines], n1, n2, edge_cost);
        num_lines++;

        edge_setter(&edges[num_lines], n2, n1, edge_cost);
        num_lines++;

        name1_exists = exister(nodes, num_nodes, n1);
        name2_exists = exister(nodes, num_nodes, n2);

        if (!name1_exists)   
        {
            if (strcmp(choice_start, n1) == 0)  
            {
               node_setter(&nodes[num_nodes], n1 ,true, false, false, 0, "");
            }
            else if (strcmp(choice_end, n1) == 0)     
            {
             node_setter(&nodes[num_nodes], n1 ,false, true, false, 1000, "");
                last_obj_index = num_nodes;
            }
            else   
            {
                node_setter(&nodes[num_nodes], n1 ,false, false, false, 1000, "");
            }

            num_nodes++;
        }
        if (!name2_exists)   
        {

            if (strcmp(choice_start, n2) == 0)  
            {
                node_setter(&nodes[num_nodes], n2 ,true, false, false, 0, "");
            }
            else if (strcmp(choice_end, n2) == 0)     
            {
              node_setter(&nodes[num_nodes], n2 ,false, true, false, 1000, "");
              last_obj_index = num_nodes;
            }
            else     
            {
            node_setter(&nodes[num_nodes], n2 ,false, false, false, 1000, "");
            }

            num_nodes++;
        }

    }


    for(int i = 0; i < num_nodes; i++)
    {
        if(nodes[i].is_start == true)
        {
            start_index = i;
        }
    }


    rewind(fpointer); 

    char n1[2];
    char n2[2];

    while(!feof(fpointer))   
    {

        Node obj1;
        Node obj2;

        int edge_cost;
        fscanf(fpointer, "%s %s %d", n1, n2, &edge_cost); 

        int obj1_index = -2;
        int obj2_index = -2;

        for (int i = 0; i < num_nodes; i++)     
        {
            if (strcmp(nodes[i].name, n1) == 0)
            {
                obj1_index = i;
                obj1 = nodes[i];  
            }
            if (strcmp(nodes[i].name, n2) == 0)
            {
                obj2_index = i;
                obj2 = nodes[i]; 
            }
        }
        
        bool has_neighbor1 = false;
        bool has_neighbor2 = false;

        for(int i = 0; i < obj1.num_neighbors; i++)
        {
            if(strcmp(obj1.neighbors[i], obj2.name) == 0)
            {
                has_neighbor1 = true;
            }
        }
        for(int i = 0; i < obj2.num_neighbors; i++)
        {
            if(strcmp(obj2.neighbors[i], obj1.name) == 0)
            {
                has_neighbor1 = true;
            }
        }

        if(has_neighbor1 == false)
        {
            strcpy(nodes[obj1_index].neighbors[obj1.num_neighbors], obj2.name);
            nodes[obj1_index].num_neighbors++;
        }
        if(has_neighbor2 == false)
        {
            strcpy(nodes[obj2_index].neighbors[obj2.num_neighbors], obj1.name);
            nodes[obj2_index].num_neighbors++;
        }

    }
    fclose(fpointer);

    Node temp_val = nodes[0];
    nodes[0] = nodes[start_index];
    nodes[start_index] = temp_val;


    current = &nodes[0];



    while (num_visited < num_nodes)
    {
        int temp_min = 100000;
        Node* temp_min_node;

        for (int j = 0; j < (*current).num_neighbors; j++)    
        {
            char current_neighbor_name[2];
            strcpy(current_neighbor_name, (*current).neighbors[j]); 
            int current_neighbor_real_index = -2; 

            for(int i = 0; i < num_nodes; i++)
            {

                if(strcmp(nodes[i].name,(current_neighbor_name))== 0)      
                {
                    current_neighbor_real_index = i;
                }
            }

            int curr_neighbor_cost = nodes[current_neighbor_real_index].cost_from_start;
            int curr_node_cost = (*current).cost_from_start;
            int edge_to_neighbor_cost;

            
            for(int i = 0; i < num_lines; i++)
            {
                if( (strcmp(edges[i].name1, (*current).name) == 0) && (strcmp(edges[i].name2, current_neighbor_name) ==0) )  
                {
                    edge_to_neighbor_cost = edges[i].cost_of_edge;
                }
            }

            (*current).visited = true; 

            if ( curr_neighbor_cost > curr_node_cost + edge_to_neighbor_cost && ((nodes[current_neighbor_real_index].visited) == false) )  
            {
                nodes[current_neighbor_real_index].cost_from_start = curr_node_cost + edge_to_neighbor_cost; 
                strcpy(nodes[current_neighbor_real_index].prev_name, (*current).name); 
            }
        }
        num_visited++;

        for (int i = 1; i < num_nodes; i++)   
        {
            if (i != start_index)  
            {

                if (nodes[i].cost_from_start < temp_min && (nodes[i].visited == false))   
                {
                    temp_min = nodes[i].cost_from_start;
                    temp_min_node = &nodes[i];
                }
            }

        }

        (current) = temp_min_node; 
        start_index++;


    }


    start_index -= num_nodes;
    char str_result[50]; 
    char str_result_flipped[50]; 
    int str_result_max_index = 0;
    int result_cost;



    for(int i = 0; i < num_nodes; i++)
    {
        if(nodes[i].is_end)
        {
            last_obj_index = i;
            break;
        }
    }

    result_cost = nodes[last_obj_index].cost_from_start;

    if(strcmp(choice_end, choice_start)!=0)
    {
        char current_obj_name[2];
        Node* current_obj = &nodes[last_obj_index]; 
        Node* prev;
        char previous_name[2];
        strcpy( previous_name,(*current_obj).prev_name);

        for(int i = 0; i < num_nodes; i++)  
        {
            if(strcmp(previous_name,nodes[i].name) == 0)
            {
                current_obj = &nodes[i];
               
            }
        }

        while( (strcmp((*current_obj).name, choice_start) != 0) )  
        {
            strcat(str_result, ">");
            str_result_max_index++;
            strcat(str_result, "-");
            str_result_max_index++;
            strcat(str_result, (*current_obj).name);
            str_result_max_index++;

            for(int i = 0; i < num_nodes; i++) 
            {
                if(strcmp((*current_obj).prev_name,nodes[i].name) == 0)
                {
                    current_obj = &nodes[i];
                    break;  
                }
            }

        }

        strcat(str_result, ">");
        str_result_max_index++;
        strcat(str_result, "-");
        str_result_max_index++;
        strcat(str_result, choice_start);

        int index = 0;

        for(int i = str_result_max_index; i >= 0; i--)
        {
            str_result_flipped[index] = str_result[i];
            index++;
        }
        strcat(str_result_flipped,nodes[last_obj_index].name);

    }
    else
    {
        result_cost = 0;
        strcpy(str_result_flipped, choice_start);
        strcat(str_result_flipped,"->");
        strcat(str_result_flipped, choice_end);
    }

    printf("\n");
    printf("%s", "Total costo por Routing = ");
    printf("%d", result_cost);
    printf("\n");
    printf("%s", str_result_flipped);
    printf("\n");

    return 0;

}

bool exister(Node* arr, int arr_size, char target[2]){
    bool result = false; 
            for(int i = 0; i < arr_size; i++) 
            {
                if(strcmp(arr[i].name,target) == 0)
                {
                    result = true;
                }

            }
    return result;
}

void node_setter(Node* n, char name[2] ,bool start, bool end, bool visited, int inital_cost, char prev[2]){
    strcpy((*n).name,name);
    (*n).is_start = start;
    (*n).is_end = end;
    (*n).visited = visited;
    (*n).cost_from_start = inital_cost;
    strcpy((*n).prev_name, prev);
}

void edge_setter(Edge* e, char name1[2], char name2[2], int edge_cost){
    strcpy((*e).name1, name1);
    strcpy((*e).name2, name2);
    (*e).cost_of_edge = edge_cost;
}






