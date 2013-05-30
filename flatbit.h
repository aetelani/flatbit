
typedef struct { int version; } Header;

typedef struct	{ int key; } Key;

typedef struct Data { int data; } Data;

typedef struct { Key key; Data data; } Record;

typedef struct { Key key; int index; } Index;

typedef struct Tables { int id; } Table;
