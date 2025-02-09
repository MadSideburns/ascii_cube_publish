Just donwnload everything to a folder, type "make all" in the terminal and watch the magic happen!

Look at cube_animation.gif to see what is must be EXACTLY like!


INFORMAZIONI UTILI
------------------

Classi definite:
- `Vector3D`: classe che rappresenta vettori in 3 dimensioni. Supporta tutte le operazioni tra vettori inclusi prodotto scalare `.dot()` e vettore `.cross()`. Sintassi del costruttore: `Vector3D(x, y, z)`. Supporta initializer lists e accesso ai membri tramite `operator[]`.
- `RotationMatrix`: classe che rappresenta una matrice di rotazione. Supporta composizione di due rotazioni e azione su un `Vector3D` usando l'operatore `*`. Sintassi del costruttore: `RotationMatrix(Vector3D asse, float angolo)`.
- `Face`: rappresenta una faccia del cubo. Costruito con `Face(Vector3D centro, Vector3D asse1, Vector3D asse2)`. Se i due assi non sono ortogonali vengono automaticamente ortogonalizzati e successivamente viene calcolata la normale, che punta nella direzione di `asse1.cross(asse2)`.
- `Cube`: rappresenta l'intero cubo. Costruito con `Cube(Vector3D centro, Vector3D asse1, Vector3D asse2, Vector3D asse3)`. Gli assi vengono ortogonalizzati come prima, e le 6 facce vengono generate automaticamente e sono storate in un array `std::array<Face, 6>`. Può essere traslato e ruotato con i metodi `.translate(Vector3D offset)` e `.rotate(RotationMatrix rotation)`. La rotazione si intende intorno al centro del cubo.
- `Sky`: descrive il cielo della scena. Contiene il livello di luce diffusa e un metodo `.get_color(Vector3D direction)` che restituisce il colore che si vede in una data direzione.
- `Ray`: raggio di luce, descritto tramite `Ray(Vector3D origine, Vector3D direzione)`. Il metodo `.get_intersection(Cube cube)` restituise l'indice della faccia del cubo intersecata, altrimenti -1 se non interseca il cubo. Il metodo `get_color(Face face, Vector3D sun_direction, Sky sky)` restituisce il colore visto se il raggio interseca la faccia passata alla funzione, con una data direzione del sole e dato il cielo (usato per i riflessi speculari).
- `Camera`: Oggetto dal quale viene scattata l'immagine. Descritto dal costruttore `Camera(int resx, int resy, Vector3D origine, Vector3D distanza_origine_pianofocale, float FOV, float aspect_ratio)`. Si può traslare e ruotare con metodi analoghi a `Cube`. Può scattare una foto e restituirla come frame buffer nella forma di un oggetto `std::ostringstream` usando il metodo `.snapshot(Cube cube, Vector3D sun_dir, Sky sky)`.

Per comodità, sono definiti i vettori `ex`, `ey`, `ez` della base cartesiana dello spazio.
