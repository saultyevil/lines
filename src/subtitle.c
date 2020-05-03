/** ************************************************************************* */
/**
 * @file     subtitle.c
 * @author   Edward Parkinson
 * @date     May 2020
 *
 * ************************************************************************** */

#include <time.h>
#include <stdlib.h>

#include "atomix.h"

/* ************************************************************************** */
/**
 * @brief   Get a random subtitle
 *
 * @return  A random subtitle
 *
 * @details
 *
 * ************************************************************************** */

char *
get_random_subtitle (void)
{
  char *sub;
  char *subtitles[] = {
    "To boldly probe atomic data where others simply don't dare or want to",
    "Digging too deep since september 2020",
    "Not as confusingly named as Python",
    "Attempting to demystify mysterious data",
    "Not for Stuart to play with",
    "This subtitle is random",
    "Good luck!",
    "Helping you plumb new depths of atomic misery :-(",
    "https://github.com/saultyevil/atomix",
    "As simple as 4D Chess",
    "I have put you on a permanent ignore, public and private",
    "From this Ghastly Eyrie I can see to the ends of the world, and I declare with utter certainty, that this one is in the bag!",
    "People say nothing is impossible, but I do nothing every day",
    "I don't believe in astrology; I'm a Sagittarius and skeptical",
    "Good enough for government work",
    "I have just been to see Her Majesty the Queen, and I will now form a government",
    "Metal Gear?",
    "Liquid!",
    "Snake, did you like my sunglasses?",
    "Is Nintendo real?",
    "Dame da ne, dame yo dame na no yo, anta ga suki de suki sugite",
    "INVALID MIT_MAGIC_COOKIE-1",
    "Some stuff about special relativity",
    "No! I want to appeal! I am TERRIFYINGLY EVIL!",
    "If only you knew the power of Bob",
    "Trimming armour",
    "Selling polished buttons 2m",
    "w00t",
    "Enemy APC spotted",
    "Enemy boat spotted",
    "Whoever wins.. is not the loser?",
    "https://github.com/agnwinds/python/",
    "The internet is not a big truck. It's a series of tubes",
    "All your base are belong to us",
    "The original cowboy builder ;-)",
    "Literal vampire potbelly goblins are hobbling around coming after us",
    "I've gotta return some video tapes",
    "I have to return some video tapes",
    "I guess I was probably returning video tapes",
    "Did you know I'm utterly insane?",
    "I'm a dude, playing a dude, disguised as another dude",
    "Gorlami",
    "Nerf Ash",
    "COVID-19 friendly!",
    "pingers",
    "This is garbage...",
    "Tim Allen grunt",
    "Remember to disable HyperThread (tm) if you using an Intel (R) CPU",

  };

  srand (time (NULL));
  sub = subtitles[rand() % ARRAY_SIZE(subtitles)];

  return sub;
}
