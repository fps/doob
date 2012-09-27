#ifndef DOOB_NOTE_HH
#define DOOB_NOTE_HH

#include <types.h>
#include <event.h>

struct note_track;
typedef double note_t;
typedef double velocity_t;

/*
 * This type is only in the hierarchy to 
 * make dynamic casts easier to write 
 * (to check compatibility with a pattern/track)
 */
struct note_track_event : event {

};

typedef shared_ptr<note_track_event> note_ptr;

struct note_on : note_track_event {
	note_t note;
	velocity_t velocity;
};

struct note_off : note_track_event {
	velocity_t velocity;
};

#endif
