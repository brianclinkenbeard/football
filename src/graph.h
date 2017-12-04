#ifndef DFS_AND_BFS_GRAPH_H
#define DFS_AND_BFS_GRAPH_H

#include <iostream>
#include <list>
#include <string>
#include <QVector>
#include <queue>
#include <QSqlQuery>

enum EdgeType {DISCOVERY,BACK,NOTHING,REPEAT,CROSS};

struct Edge
{
    int weight;
    QString destination;
    EdgeType edgeType;
};

struct Vertex
{
    bool visited;
    QString name;
    QVector<Edge> edgeList;
    int cost;
};

class CompareCost // a cost comparator
{
public:
    bool operator()(const Vertex& p, const Vertex& q) const
    { return (p.cost > q.cost);}
};


template<class Type>
class Graph
{
public:
    Graph();
    ~Graph();

    void addEdge(Type vertex1, Type vertex2, int weight);
    void addVertex(Type vertex);
    bool vertexExists(Type vertex);
    int getWeight(Type vertex1, Type vertex2);
    void printAdjList();

    void DFS(Type vertex);
    void BFS(Type vertex);
    void Dijkstra(Type vertex);
    void MST(Type vertex);
    void recursiveDijkstra(Type vertex);

    void printEdgeListType();

    void loadGraph(Graph &g);
    QVector<QString> getOrder();
    int getTotalDistance();
    int getCost(Type vertex);

private:

    int findSmallestEdgeIndex(QVector<Edge> *ptr);
    int findVertexIndex(Type vertex);

    bool checkAvailableVertices(Type vertex);
    bool checkAvailableVertices(int adjListIndex);

    void DFS(int index);
    void BFS(int index);

    void setReverse(Type vertex1, Type vertex2);
    void setDiscovery(Type vertex1, Type vertex2);

    int findSmallest();
    void recursiveDijkstra(Type vertex,int position);

    QList<int> getPath(int startVertex,int endVertex,QVector<int> parent);
    void clearVisitedVertex();
    void clearEdgeType();

    int numberOfVertex;
    QVector<Vertex> adjList;
    int totalDistance;
    int location;

    QVector<QString> order;

    QSqlDatabase db = QSqlDatabase::database();

};

template <class Type>
Graph<Type>::Graph()
{
    numberOfVertex = 0;
    totalDistance = 0;
    adjList.clear();
}

template <class Type>
Graph<Type>::~Graph()
{

}
template <class Type>
QVector<QString> Graph<Type>::getOrder(){
    return order;
}

template <class Type>
void Graph<Type>::addEdge(Type vertex1, Type vertex2, int weight)
{
    if(adjList.size() > 0) {
        if(vertexExists(vertex1) && vertexExists(vertex2)) {
            Edge newEdge;
            newEdge.destination = vertex2;
            newEdge.weight = weight;
            newEdge.edgeType = NOTHING;

            for(int i = 0; i < adjList.size(); i++) {
                if(adjList[i].name == vertex1) {
                    adjList[i].edgeList.push_back(newEdge);
                }
            }
        }
        else {
            qDebug() << "Vertex's did not even exist";
        }
    }
}

template <class Type>
void Graph<Type>::addVertex(Type vertex)
{
    Vertex newVertex;
    newVertex.name = vertex;
    newVertex.visited = false;

    adjList.push_back(newVertex);
    numberOfVertex++;
}

template <class Type>
bool Graph<Type>::vertexExists(Type vertex)
{
    bool found = false;

    for(int i = 0; i < adjList.size(); i++) {
        if(adjList[i].name == vertex) {
            found = true;
            break;
        }
    }
    return found;
}

template <class Type>
int Graph<Type>::getWeight(Type vertex1, Type vertex2)
{
    for(int i = 0; i < adjList.size(); i++) {
        if(adjList[i].name == vertex1) {
            for(int j = 0; j < adjList[i].edgeList.size(); j++) {
                if(adjList[i].edgeList[j].destination == vertex2) {
                    return adjList[i].edgeList[j].weight;
                }
            }
        }
    }
    return -1;
}

template <class Type>
void Graph<Type>::printAdjList()
{
    for(int i = 0; i < adjList.size(); ++i){
        qDebug() << "Start City: " << adjList[i].name << endl;
        for(int j=0; j<adjList[i].edgeList.size(); ++j){
            qDebug() << "End City: " << adjList[i].edgeList[j].destination << endl;
            qDebug() << "Weight: " << adjList[i].edgeList[j].weight << endl;
        }
        qDebug() << "\n\n";
    }
}


template <class Type>
int Graph<Type>::findVertexIndex(Type vertex)
{
    for(int i=0; i<adjList.size(); ++i){
        if(adjList[i].name == vertex) return i;
    }
    return -1;
}

//Same function but different call methods
/////////////////////////////////////////////////////////////////////////////////////////////////
template <class Type>
bool Graph<Type>::checkAvailableVertices(Type vertex)
{
    //gets the edge list of the specified vertex
    QVector<Edge> *ptr = &(adjList[findVertexIndex(vertex)].edgeList);

    //return true if at least one destination in the vertex list is not visited.
    for(int i=0; i<ptr->size(); ++i){
        if(!adjList[findVertexIndex((*ptr)[i].destination)].visited) {
            return true;
        }


    }

    return false;
}

template <class Type>
bool Graph<Type>::checkAvailableVertices(int adjListIndex)
{
    //gets the edge list of the specified vertex
    QVector<Edge> *ptr = &(adjList[adjListIndex].edgeList);

    //return true if at least one destination in the vertex list is not visited.
    for(int i=0; i<ptr->size(); ++i){
        if(!adjList[findVertexIndex((*ptr)[i].destination)].visited) {
            return true;
        }
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

template <class Type>
int Graph<Type>::findSmallestEdgeIndex(QVector<Edge> *ptr)
{
    int index = 0;
    int weight = 0;
    //finds first edge with an unvisited end vertex
    for(int i=0; i<ptr->size(); ++i){
        if(!adjList[findVertexIndex((*ptr)[i].destination)].visited) {
            index = i;
            weight = (*ptr)[i].weight;
            break;
        }
    }

    //searches the rest of the edge list for the edge with the smallest weight
    for(int i=index+1; i<ptr->size(); ++i){
        if(!adjList[findVertexIndex((*ptr)[i].destination)].visited && (*ptr)[i].weight < weight) {
            index = i;
            weight = (*ptr)[i].weight;
        }
    }

    return index;
}

template <class Type>
void Graph<Type>::clearVisitedVertex()
{
    for(int i=0; i<adjList.size(); ++i){
        adjList[i].visited = false;
    }
}

template <class Type>
void Graph<Type>::clearEdgeType()
{
    for (int i = 0; i < adjList.size(); ++i) {
        for(int j = 0; j < adjList[i].edgeList.size(); j++){
            adjList[i].edgeList[j].edgeType = NOTHING;
        }
    }
}

template <class Type>
void Graph<Type>::DFS(Type vertex)
{
    order.clear();
    totalDistance = 0;
    this->DFS(findVertexIndex(vertex));

    //Set Everything not used to back edges since they can no longer discover anything
    for (int i = 0; i < adjList.size(); ++i) {
        for(int j = 0; j < adjList[i].edgeList.size(); j++) {
            if(adjList[i].edgeList[j].edgeType != DISCOVERY && adjList[i].edgeList[j].edgeType != REPEAT) {
                adjList[i].edgeList[j].edgeType = BACK;
            }
        }
    }
    //Grabs total distance traveled
    totalDistance = getTotalDistance();
    qDebug() << "End\n";
    qDebug() << "Total Distance traveled: " << totalDistance << endl;
    this->clearVisitedVertex();
    this->clearEdgeType();
}

template <class Type>
void Graph<Type>::DFS(int index){
    adjList[index].visited = true;
    qDebug() << adjList[index].name << " --> ";
    order.append(adjList[index].name);

    while(checkAvailableVertices(index)){
        int edgeIndex = this->findSmallestEdgeIndex(&(adjList[index].edgeList));
        int vertexIndex = this->findVertexIndex(adjList[index].edgeList[edgeIndex].destination);

        //If the edge has never been used before set it to discovery!
        if(adjList[index].edgeList[edgeIndex].edgeType == NOTHING) {
            adjList[index].edgeList[edgeIndex].edgeType = DISCOVERY;
            setReverse(adjList[index].name,adjList[index].edgeList[edgeIndex].destination);
        }

        //recursively call the DFS algorithm starting from the new vertex
        this->DFS(vertexIndex);
    }
}

template <class Type>
void Graph<Type>::BFS(Type vertex)
{
    order.clear();
    totalDistance = 0;
    this->BFS(findVertexIndex(vertex));
    totalDistance = getTotalDistance();

    for (int i = 0; i < adjList.size(); ++i) {
        for(int j = 0; j < adjList[i].edgeList.size(); j++) {
            if(adjList[i].edgeList[j].edgeType != DISCOVERY && adjList[i].edgeList[j].edgeType != REPEAT) {
                adjList[i].edgeList[j].edgeType = CROSS;
            }
        }
    }

    qDebug() << "End\n";
    this->clearVisitedVertex();
    this->clearEdgeType();
    qDebug() << "Total Distance traveled: " << totalDistance << endl;
}

template <class Type>
void Graph<Type>::BFS(int index)
{
    adjList[index].visited = true;
    qDebug() << adjList[index].name << " --> ";
    order.append(adjList[index].name);

    QVector<int> indexList;

    //visits all adjacent vertices from one vertex
    //adds the adjacent vertices to a new vector
    while(checkAvailableVertices(index)){
        int edgeIndex = this->findSmallestEdgeIndex(&(adjList[index].edgeList));
        int vertexIndex = this->findVertexIndex(adjList[index].edgeList[edgeIndex].destination);
        if(adjList[index].edgeList[edgeIndex].edgeType == NOTHING)
        {
            adjList[index].edgeList[edgeIndex].edgeType = DISCOVERY;
            setReverse(adjList[index].name,adjList[index].edgeList[edgeIndex].destination);
        }
        adjList[vertexIndex].visited = true;
        qDebug() << adjList[vertexIndex].name << " --> ";
        order.append(adjList[vertexIndex].name);
        indexList.push_back(vertexIndex);
    }

    //visits all adjacent vertices if unvisited from each vertex in the vertex list
    //adds any new vertex that has not been found yet
    for(int i=0; i<indexList.size(); ++i){
        while(checkAvailableVertices(indexList[i])){
            int edgeIndex = this->findSmallestEdgeIndex(&(adjList[ indexList[i] ].edgeList));
            int vertexIndex = this->findVertexIndex(adjList[ indexList[i] ].edgeList[edgeIndex].destination);
            if(adjList[indexList[i]].edgeList[edgeIndex].edgeType == NOTHING)
            {
                adjList[indexList[i]].edgeList[edgeIndex].edgeType = DISCOVERY;
                setReverse(adjList[indexList[i]].name,adjList[indexList[i]].edgeList[edgeIndex].destination);
            }

            adjList[vertexIndex].visited = true;
            qDebug() << adjList[vertexIndex].name << " --> ";
            order.append(adjList[vertexIndex].name);
            indexList.push_back(vertexIndex);
        }
    }
}

template <class Type>
void Graph<Type>::printEdgeListType()
{
    for(int i = 0; i < adjList.size(); i++) {
        for(int j = 0; j < adjList[i].edgeList.size(); j++) {
            qDebug() << adjList[i].name << " --> " << adjList[i].edgeList[j].destination
                 << " is edge type ";
            switch(adjList[i].edgeList[j].edgeType) {
                case DISCOVERY: qDebug() << "Discovery";
                    break;
                case BACK: qDebug() << "Back";
                    break;
                case NOTHING: qDebug() << "Discovery/back/cross edge already";
                    break;
                case REPEAT: qDebug() << "Discovery edge already from the reverse way";
                    break;
                case CROSS: qDebug() << "Cross";
                    break;
                default: qDebug() << "How?!?!";
            }
            qDebug() << endl;
        }
    }
}

template <class Type>
int Graph<Type>::getTotalDistance()
{
    for (int i = 0; i < adjList.size(); ++i) {
        for(int j = 0; j < adjList[i].edgeList.size(); j++) {
            if(adjList[i].edgeList[j].edgeType == DISCOVERY) {
                totalDistance += getWeight(adjList[i].name,adjList[i].edgeList[j].destination);
            }
        }
    }
    return totalDistance;
}

template <class Type>
void Graph<Type>::setReverse(Type vertex1, Type vertex2)
{
    int index = 0;
    for(int i = 0; i < adjList.size(); i++) {
        if(adjList[i].name == vertex2) {
            index = i;
        }
    }

    for(int i = 0; i < adjList[index].edgeList.size(); i++) {
        if(adjList[index].edgeList[i].destination == vertex1) {
            adjList[index].edgeList[i].edgeType = REPEAT;
        }
    }
}

template<class Type>
void Graph<Type>::loadGraph(Graph& g)
{
    QSqlQuery* graphQuery = new QSqlQuery(db);
    graphQuery->prepare("SELECT DISTINCT Start FROM Distance");
    if(graphQuery->exec()) {
        while(graphQuery->next()) {
           g.addVertex(graphQuery->value(0).toString());
           qDebug() << "Added" << graphQuery->value(0).toString();
           qDebug() << adjList.size();
        }
    }

    QSqlQuery* graphQueryEdge = new QSqlQuery(db);
    graphQueryEdge->prepare("SELECT * FROM Distance");
    if(graphQueryEdge->exec()) {
        while(graphQueryEdge->next()) {
            g.addEdge(graphQueryEdge->value(0).toString(),graphQueryEdge->value(1).toString(),graphQueryEdge->value(2).toInt());
            qDebug() << "Start:" << graphQueryEdge->value(0).toString();
            qDebug() << "End:" << graphQueryEdge->value(1).toString();
            qDebug() << "Weigt:" << graphQueryEdge->value(2).toInt();
        }
    }
}

template <class Type>
void Graph<Type>::Dijkstra(Type vertex)
{
    Vertex u;
    QVector<int> parent(numberOfVertex);

    //Code to set the cost array
    for(int i = 0; i < adjList.size(); i++){
        adjList[i].cost = std::numeric_limits<int>::max();
    }

    adjList[findVertexIndex(vertex)].cost = 0;

    //priority queue to go through the vector
    std::priority_queue<Vertex,QVector<Vertex>,CompareCost> Q;

    int start = findVertexIndex(vertex);

    parent[start] = start;

    Q.push(adjList[start]);

    while(!Q.empty()){
        u = Q.top();
        Q.pop();
        for(int i = 0; i < adjList[findVertexIndex(u.name)].edgeList.size(); i++){
            int foundVertex = findVertexIndex(u.name);
            int foundEdgeVertex = findVertexIndex(adjList[foundVertex].edgeList[i].destination);
            if(adjList[foundVertex].cost + getWeight(adjList[foundVertex].name,adjList[foundEdgeVertex].name)
               < adjList[foundEdgeVertex].cost){
                adjList[foundEdgeVertex].cost = adjList[foundVertex].cost +
                                                getWeight(adjList[foundVertex].name, adjList[foundEdgeVertex].name);
                parent[foundEdgeVertex] = foundVertex;
                Q.push(adjList[foundEdgeVertex]);
            }
        }
    }


    for(int i = 0; i < adjList.size(); i++){
        qDebug() << "Cost is " << adjList[i].cost << " to go from "
             << adjList[start].name << " to " << adjList[i].name << endl;
    }


}


template <class Type>
void Graph<Type>::MST(Type vertex)
{
    Vertex u;
    int distance = 0;

    QVector<QString> parent(numberOfVertex);
    QVector<bool> isInMST(numberOfVertex);

    //Code to set the cost array
    for(int i = 0; i < adjList.size(); i++){
        adjList[i].cost = std::numeric_limits<int>::max();
    }

    adjList[findVertexIndex(vertex)].cost = 0;
    parent[findVertexIndex(vertex)] = adjList[findVertexIndex(vertex)].name;

    //priority queue to go through the vector
    std::priority_queue<Vertex,QVector<Vertex>,CompareCost> Q;

    Q.push(adjList[findVertexIndex(vertex)]);

    while(!Q.empty()){
        u = Q.top();
        Q.pop();

        isInMST[findVertexIndex(u.name)] = true;

        for(int i = 0; i < adjList[findVertexIndex(u.name)].edgeList.size(); i++){
            int foundVertex = findVertexIndex(u.name);
            int foundEdgeVertex = findVertexIndex(adjList[foundVertex].edgeList[i].destination);
            if(getWeight(adjList[foundVertex].name,adjList[foundEdgeVertex].name)
               < adjList[foundEdgeVertex].cost && !isInMST[foundEdgeVertex]){
                adjList[foundEdgeVertex].cost = getWeight(adjList[foundVertex].name, adjList[foundEdgeVertex].name);
                parent[foundEdgeVertex] = adjList[foundVertex].name;
                Q.push(adjList[foundEdgeVertex]);
            }
        }
    }

    for(int i = 0; i < adjList.size(); i++){
        qDebug() << "Cost is " << adjList[i].cost
             << " to go from " << parent[i]
             << " to " << adjList[i].name << endl;

        distance += adjList[i].cost;
    }

    qDebug() << "Total Distance is " << distance;
}

template<class Type>
int Graph<Type>::getCost(Type vertex)
{
    int position = findVertexIndex(vertex);

    return adjList[position].cost;
}

template <class Type>
void Graph<Type>::recursiveDijkstra(Type vertex)
{
    recursiveDijkstra(vertex,1);
}

template <class Type>
void Graph<Type>::recursiveDijkstra(Type vertex,int position)
{
    adjList[findVertexIndex(vertex)].visited = true;
    if(position != numberOfVertex){
        Dijkstra(vertex);
        location = findSmallest();
        totalDistance += adjList[location].cost;
        qDebug() << "Closest Stadium: " << adjList[location].name;
        recursiveDijkstra(adjList[location].name,position+1);
    }
    else{
        totalDistance += adjList[location].cost;
    }
}

template <class Type>
int Graph<Type>::findSmallest()
{
    int smallestCost = 999999;
    int smallestIndex;

    for(int i = 0; i < adjList.size(); i++){
        if(adjList[i].cost < smallestCost && adjList[i].cost != 0 && !adjList[i].visited){
            smallestCost = adjList[i].cost;
            smallestIndex = i;
        }
    }
    return smallestIndex;
}

#endif //DFS_AND_BFS_GRAPH_H
