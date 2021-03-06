#include "SFMLJeu.h"

using namespace sf;

void Init(Hero* h, Contents* content, Map* m, std::vector<Ennemi*> &ennemis, std::vector<AnimatedSprite> &tEnnemis, std::vector<Vector2f> &movementE) {
  m->initGeneration();
  m->positionnement(h, ennemis);
  h->setSprites(content->anim_Joueur, content->tHero[1], content->tHero[2]);
  h->PvPos();
  for(unsigned int i = 0; i < ennemis.size(); i++) {
    tEnnemis.push_back(AnimatedSprite(seconds(0.2), true));
    tEnnemis[i].setPosition(Vector2f(ennemis[i]->getX()*32, ennemis[i]->getY()*32));
    movementE.push_back(Vector2f(0.f, 0.f));
    ennemis[i]->setSprites(content->anim_Ennemie, content->tEnnemi[10], content->tEnnemi[11]);
    ennemis[i]->PvPos();
  }
}

void Boucle (Hero *h) {
  Vector2i screenDimensions(720, 720);
  RenderWindow window(VideoMode(screenDimensions.x, screenDimensions.y), "Rogue");
  window.setFramerateLimit(60);
  Contents* content = new Contents;
  Map* m = new Map;
  std::vector<Ennemi*> ennemis;
  std::vector<AnimatedSprite> tEnnemis;
  std::vector<Vector2f> movementE;
  Sprite tab[90][90];
  Init(h, content, m, ennemis, tEnnemis, movementE);
  int taille = m->size();
  ajoutTexture(content, m, tab);
  View view(Vector2f(h->getX()*32, h->getY()*32), Vector2f(screenDimensions/2));
  View minimap(Vector2f(h->getX()*32, h->getY()*32), Vector2f(taille*32, taille*32));
  minimap.setViewport(FloatRect(0.75f, 0.f, 0.25f, 0.25f));
  AnimatedSprite hero(seconds(0.2), true);
  hero.setPosition(Vector2f(h->getX()*32, h->getY()*32));
  Clock frameClock;
  float speed = 2.f;
  bool noKeyWasPressed = true;
  Vector2f movementH(0.f, 0.f);
  int pasH = 0;
  int pasE = 0;
  while (window.isOpen())
  {
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            window.close();
    }
    Time frameTime = frameClock.restart();
    if(h->getPv() > 0) {
      if(pasH == 0){
        m->setValueMap(h->getX(),h->getY());
        if (Keyboard::isKeyPressed(Keyboard::Up) &&
        m->getValueMap(h->getX(),(h->getY() - 1)) == 2) {
          h->haut();
          movementH.y -= speed;
          noKeyWasPressed = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) &&
        m->getValueMap(h->getX(),(h->getY() + 1)) == 2) {
          h->bas();
          movementH.y += speed;
          noKeyWasPressed = false;
        }
        else if(Keyboard::isKeyPressed(Keyboard::Left) &&
        m->getValueMap((h->getX() - 1),h->getY()) == 2) {
          h->gauche();
          movementH.x -= speed;
          noKeyWasPressed = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right) &&
        m->getValueMap((h->getX() + 1),h->getY()) == 2)
        {
          h->droite();
          movementH.x += speed;
          noKeyWasPressed = false;
        }
        m->setValueMap(h->getX(),h->getY(), 3);
        unsigned int idE = 0;
        while(idE < ennemis.size() && !(h->combat(ennemis[idE])))
          idE++;
      }
      if(pasE == 0)
        for(unsigned int i = 0; i < ennemis.size(); i++) {
          m->setValueMap(ennemis[i]->getX(),ennemis[i]->getY());
          if(ennemis[i]->getPv() > 0) {
            movementE[i] = ennemis[i]->update(m->getValueMap(ennemis[i]->getX(),(ennemis[i]->getY() - 1)),
              m->getValueMap(ennemis[i]->getX(),(ennemis[i]->getY() + 1)),
              m->getValueMap((ennemis[i]->getX() - 1),ennemis[i]->getY()),
              m->getValueMap((ennemis[i]->getX() + 1),ennemis[i]->getY()), h);
            m->setValueMap(ennemis[i]->getX(),ennemis[i]->getY(), 4);
          }
        }
    }
    pasE++;
    if(movementH != Vector2f(0.f,0.f))
      pasH += speed;
    view.move(movementH);
    minimap.move(movementH);
    hero.play(h->getSprite());
    hero.move(movementH);
    hero.update(frameTime);
    h->movePv(Vector2f(movementH.x,movementH.y));
    for(unsigned int i = 0; i < ennemis.size(); i++) {
      tEnnemis[i].play(ennemis[i]->getSprite());
      tEnnemis[i].move(movementE[i]);
      tEnnemis[i].update(frameTime);
      ennemis[i]->movePv(Vector2f(movementE[i].x,movementE[i].y));
    }
    window.setView(view);
    if(pasE == 32.f) {
      pasE = 0;
      for(unsigned int i = 0; i < ennemis.size(); i++) {
        movementE[i] = Vector2f(0.f,0.f);
        if(ennemis[i]->getPv() == 0) {
          m->setValueMap(ennemis[i]->getX(),ennemis[i]->getY());
          delete ennemis[i];
          ennemis.erase(ennemis.begin()+i);
          tEnnemis.erase(tEnnemis.begin()+i);
          movementE.erase(movementE.begin()+i);
          i--;
        }
      }
    }
    if (noKeyWasPressed && pasH == 32.f)
    {
      pasH = 0;
      movementH = Vector2f(0.f,0.f);
      View view(Vector2f(h->getX()*32, h->getY()*32), Vector2f(screenDimensions/2));
      View minimap(Vector2f(h->getX()*32, h->getY()*32), Vector2f(taille*32, taille*32));
      hero.setPosition(Vector2f(h->getX()*32, h->getY()*32));
      h->idle();
    }
    noKeyWasPressed = true;
    // draw
    window.clear();
    for (int x = 0; x < taille; x++)
        for (int y = 0; y < taille; y++)
          window.draw(tab[x][y]);
    window.draw(hero);
    window.draw(h->getPvBarre());
    window.draw(h->getCurrentPV());
    for(unsigned int i = 0; i < ennemis.size(); i++) {
      window.draw(tEnnemis[i]);
      window.draw(ennemis[i]->getPvBarre());
      window.draw(ennemis[i]->getCurrentPV());
    }
    window.setView(minimap);
    for (int x = 0; x < taille; x++)
        for (int y = 0; y < taille; y++)
          window.draw(tab[x][y]);
    window.display();
  }
  system("clear");
  Detruit(content, m, ennemis, tEnnemis, movementE);
}

void Detruit(Contents* content, Map* m, std::vector<Ennemi*> &ennemis, std::vector<AnimatedSprite> &tEnnemis, std::vector<Vector2f> &movementE){
    delete m;
    delete content;
    for(unsigned int i = 0; i < ennemis.size(); i++)
      delete ennemis[i];
    ennemis.clear();
    tEnnemis.clear();
    movementE.clear();
    m = NULL;
}


void ajoutTexture(Contents* content, Map* m, Sprite tab[90][90]) {
  int taille = m->size();
  Vector2i solPossible[11];
  solPossible[0] = Vector2i(256,160);
  solPossible[1] = Vector2i(256,192);
  solPossible[2] = Vector2i(288,160);
  solPossible[3] = Vector2i(288,192);
  solPossible[4] = Vector2i(384,160);
  solPossible[5] = Vector2i(416,160);
  solPossible[6] = Vector2i(353,320);
  solPossible[7] = Vector2i(384,320);
  solPossible[8] = Vector2i(416,320);
  solPossible[9] = Vector2i(384,288);
  solPossible[10] = Vector2i(384,352);
  int solPris;
  bool special = (bool)(rand() % 2);
  for (int x = 0; x < taille; x++){
    std::cout << '\n';
      for (int y = 0; y < taille; y++){
          int type = m->getValueMap(x, y);
          std::cout << type;
          tab[x][y].setTexture(*(content->tJeu));
          if (type == 0){
              tab[x][y].setTextureRect(IntRect(224, 320, 32, 32));
          }
          else if (type == 1){
              tab[x][y].setTextureRect(IntRect(36, 84, 32, 32));
          }
          else if (type == 2){
            if(special)
              solPris = rand() % 6;
            else
              solPris = rand() % (11-6) + 6;
            tab[x][y].setTextureRect(IntRect(solPossible[solPris].x, solPossible[solPris].y, 32, 32));

          }
          tab[x][y].setPosition(x*32, y*32);
      }
  }
}
