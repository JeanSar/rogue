#include <iostream>
#include <cmath>
#include "Room.h"

using namespace std;
//on créer un constante PI pour les calculs utilisant de la trigonométrie
long double const PI = 3.1415926535897932384626433832795028841968;

//génère un double aléatoire
double genRandomDouble() {
    return ((double)rand() / (RAND_MAX));
}

//ACCESSEUR
int Room::getIDlinked() {
    return IDlinked;
}
float Room::getx0() {
    return x0;
}
float Room::gety0() {
    return y0;
}


int Room::getX0() {
    return X0;
}
int Room::getY0() {
    return Y0;
}


int Room::getX() {
    return X;
}
int Room::getY() {
    return Y;
}
int Room::getH() {
    return H;
}
int Room::getL() {
    return L;
}
//MUTATEUR
void Room::setIDlinked(int const val) {
    IDlinked = val;
}

void Room::setx0(float const val) {
    x0 = val;
}

void Room::sety0(float const val) {
    y0 = val;
}


void Room::arrondValRoom() {
    //on arrondi les valeurs des rooms
    X0 = round(x0);
	Y0 = round(y0);
    //on initialise les valeurs des centres des rooms
    initCenterRooms();
}

const bool Room::isRoomIn(int const map_size) {
    //vérifie si la room est dans la map
    if ((X0 >= 0) && (Y0 >= 0) && ((X0 + L - 1) < map_size) && ((Y0 + H - 1) < map_size)) return true;
    else return false;
}


void Room::genInCircle(int const map_size, int const radius, int const room_max_size, int const room_min_size) {
    //on genere des tailles aléatoire
    H = (rand() % (room_max_size - room_min_size)) + room_min_size;//hauteur
    L = (rand() % (room_max_size - room_min_size)) + room_min_size;//largeur

    //on genere des points aléatoires dans un cercle
    float t = 2 * PI * genRandomDouble();
    float u = genRandomDouble() + genRandomDouble();
    float r;
    if (u > 1) r = (2 - u);
    else r = u;
    
    x0 = (radius * r * cos(t)) + (map_size/2);
    y0 = (radius * r * sin(t)) + (map_size/2);

}

bool const Room::roomCollision(Room& A) {

    //detecte si A entre en collision avec l'objet actuelle
    if((A.getx0() >= x0 + L) // trop à droite
	|| (A.getx0() + A.L <= x0) // trop à gauche
	|| (A.gety0() >= y0 + H) // trop en bas
 	|| (A.gety0() + A.H <= y0)) // trop en haut
        return false;
    else return true;
}

void Room::initCenterRooms() {
    //on place les centres des rooms
        X = X0 + (int)(L / 2) - 1;

        Y = Y0 + (int)(H / 2) - 1;
}
