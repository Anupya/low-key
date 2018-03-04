#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <math.h>

using namespace std;

/*
1. Blues
2. Classical
3. Country
4. Electronic
5. HIp Hop
6. Industrial
7. Jazz
8. Misc
9. Pop
10. Rap
11. R&B
12. Rock
*/

struct genre
{
  int genre_rating[12] = {0};
};

struct Artist;

struct Music
{
  int position;
  string name;
  Artist * original_artist;
  struct genre associated_genre;
  int likes;
};

struct User
{
  int position;
  string name;
  struct genre profile;
  vector <Music *> listened;
};

struct Artist
{
  int position;
  string name;
  vector <Music *> songs;
  struct genre overall;
  int likes;
  int favourites;
};

User * CreateUser (string name, struct genre liking)
{
  User * newUser = new User;

  newUser->name = name;
  newUser->profile = liking;

  return newUser;
}

Music * createMusic (string name, struct genre associated, struct Artist original)
{
  Music * newSong = new Music;

  Artist * original_artist = & original;

  newSong->name = name;
  newSong->associated_genre = associated;
  newSong->original_artist = & original;
  newSong->likes = 0;

  return newSong;
}

Artist * createArtist(string name, struct genre primary)
{
  Artist * newArtist = new Artist;

  newArtist->name = name;
  newArtist->overall = primary;
  newArtist->likes = 0;
  newArtist->favourites = 0;

  return newArtist;
}

genre * enterGenre()
{
  struct genre new_genre;
  genre * new_genre_ptr;
  int transfer_genre;
  int transfer_percentage;

  cout << "Which genre(s) best defines the song/artist, enter in a percentage value as well?" << endl;
  cout << "1. Blues" << endl;
  cout << "2. Classical" << endl;
  cout << "3. Country" << endl;
  cout << "4. Electronic" << endl;
  cout << "5. Hip Hop" << endl;
  cout << "6. Industrial" << endl;
  cout << "7. Jazz" << endl;
  cout << "8. Misc" << endl;
  cout << "9. Pop" << endl;
  cout << "10. Rap" << endl;
  cout << "11. R&B" << endl;
  cout << "12. Rock" << endl << endl;
  cout << "Send input as list of commands (2 'newline' 20)" << endl;

  while(!cin.eof())
  {
    cin >> transfer_genre;
    cin >> transfer_percentage;

    new_genre_ptr->genre_rating[transfer_genre] = 1000 * transfer_percentage;
  }

  new_genre_ptr = & new_genre;
  return new_genre_ptr;
}

void enterArtist(string name, struct genre initial_given_genre, vector <Artist> & current_artists)
{
  int traverse = 0;
  bool flag = true, exists = false;
  Artist * givenArtist;

  Artist * newArtist = createArtist(name, initial_given_genre);

  current_artists.push_back(*newArtist);

  traverse = sizeof(current_artists) - 1;

  newArtist->position = traverse;
}

void updateArtist(const vector <Artist> & current_artists, int artist_position)
{
  double difference = 0, adjustment_factor = 0;
  struct genre song_genre;
  struct genre artist_genre;
  Music * current_song;
  int sum_of_genre;
  struct genre summed_genre;

  artist_genre = current_artists[artist_position].overall;

  for(int i = 0; i < sizeof(artist_genre.genre_rating); i++)
  {
    current_song = current_artists[artist_position].songs[i];

    for(int j = 0; j < sizeof(current_artists[artist_position].songs); j++)
    {
      summed_genre.genre_rating[j] += current_song->associated_genre.genre_rating[j];
    }
  }

  for(int i = 0; i < sizeof(artist_genre.genre_rating); i++)
  {
    summed_genre.genre_rating[i] = floor(summed_genre.genre_rating[i]/sizeof(artist_genre.genre_rating));
    artist_genre.genre_rating[i] = summed_genre.genre_rating[i];
  }
}

void enterMusic(string name, string name_of_artist, struct genre intialGenre, vector <Artist> & current_artists, vector <Music> & current_songs)
{
  int traverse = 0;
  bool flag = true, exists = false;
  Artist * newArtist;
  Artist * givenArtist;

  while(flag)
  {
    if(current_artists[traverse].name == name_of_artist)
    {
      flag = false;
      exists = true;
    }

    else if (traverse >= (sizeof(current_artists) - 1))
    {
      flag = false;
    }

    else
    {
      traverse ++;
    }
  }

  if(exists == false)
  {
    newArtist = createArtist(name_of_artist, intialGenre);
    current_artists.push_back(*newArtist);

    givenArtist = & current_artists[(traverse + 1)];
    givenArtist->position = traverse;
  }

  else
  {
    givenArtist = & current_artists[traverse];
  }

  Music * newSong = createMusic(name, intialGenre, *givenArtist);

  current_songs.push_back(*newSong);
  traverse = sizeof(current_songs);

  newSong->position = traverse;
  givenArtist->songs.push_back(newSong);

  updateArtist(current_artists, givenArtist->position);
}

void enterUser(string name, struct genre intialGenre, vector <User> & current_users)
{
  int traverse = 0;
  bool flag = true, exists = false;
  User * new_user;
  User * givenUser;

  new_user = CreateUser(name, intialGenre);

  current_users.push_back(new_user);

  traverse = sizeof(current_users) - 1;

  new_user->position = traverse;
}

void Like(const vector <User> & current_users, int user_position, const vector <Music> & current_songs, int song_position, vector <Artist> & current_artists, int artist_position)
{
  double difference = 0, adjustment_factor = 0;
  struct genre user_genre;
  struct genre song_genre;
  struct genre artist_genre;

  user_genre = current_users[user_position].profile;
  song_genre = current_songs[song_position].associated_genre;
  artist_genre = current_artists[artist_position].overall;

  for (int i = 0; i < sizeof(user_genre.genre_rating); i++)
  {
      difference = user_genre.genre_rating[i] - song_genre.genre_rating[i];

      if(((user_genre.genre_rating[i]) != 0) && ((song_genre.genre_rating[i]) != 0))
      {
        adjustment_factor = 1/(user_genre.genre_rating[i] + song_genre.genre_rating[i]);
      }
      else
      {
      if(difference > 0)
      {
        adjustment_factor = user_genre.genre_rating[i]/song_genre.genre_rating[i];
      }
      else
      {
        adjustment_factor = song_genre.genre_rating[i]/user_genre.genre_rating[i];
      }
    }

    updateArtist(current_artists, artist_position);

      user_genre.genre_rating[i] += floor(difference * adjustment_factor);
      song_genre.genre_rating[i] += floor(difference * adjustment_factor);

      current_artists[artist_position].likes ++;
  }
}

void Dislike(const vector <User> & current_users, int user_position, const vector <Music> & current_songs, int song_position, const vector <Artist> & current_artists, int artist_position)
{
  double difference = 0, adjustment_factor = 0;
  struct genre user_genre;
  struct genre song_genre;
  struct genre artist_genre;

  user_genre = current_users[user_position].profile;
  song_genre = current_songs[song_position].associated_genre;
  artist_genre = current_artists[artist_position].overall;

  for (int i = 0; i < sizeof(user_genre.genre_rating); i++)
  {
      difference = user_genre.genre_rating[i] - song_genre.genre_rating[i];

      if(((user_genre.genre_rating[i]) != 0) && ((song_genre.genre_rating[i]) != 0))
      {
        adjustment_factor = 1/(user_genre.genre_rating[i] + song_genre.genre_rating[i]);
      }
      else
      {
      if(difference > 0)
      {
        adjustment_factor = user_genre.genre_rating[i]/song_genre.genre_rating[i];
      }
      else
      {
        adjustment_factor = song_genre.genre_rating[i]/user_genre.genre_rating[i];
      }
    }

    updateArtist(current_artists, artist_position);

    user_genre.genre_rating[i] -= floor(difference * adjustment_factor);
    song_genre.genre_rating[i] -= floor(difference * adjustment_factor);
  }
}

void Save(vector <User> & current_users, int user_position, vector <Music> & current_songs, int song_position, vector <Artist> & current_artists, int artist_position)
{
  double difference = 0, adjustment_factor = 0;
  struct genre user_genre;
  struct genre song_genre;
  struct genre artist_genre;

  user_genre = current_users[user_position].profile;
  song_genre = current_songs[song_position].associated_genre;
  artist_genre = current_artists[artist_position].overall;

  for (int i = 0; i < sizeof(user_genre.genre_rating); i++)
  {
      difference = user_genre.genre_rating[i] - song_genre.genre_rating[i];

      if(((user_genre.genre_rating[i]) != 0) && ((song_genre.genre_rating[i]) != 0))
      {
        adjustment_factor = 1/(user_genre.genre_rating[i] + song_genre.genre_rating[i]);
      }
      else
      {
      if(difference > 0)
      {
        adjustment_factor = user_genre.genre_rating[i]/song_genre.genre_rating[i];
      }
      else
      {
        adjustment_factor = song_genre.genre_rating[i]/user_genre.genre_rating[i];
      }
    }

    updateArtist(current_artists, artist_position);

    adjustment_factor *= 10;

    user_genre.genre_rating[i] += floor(difference * adjustment_factor);
    song_genre.genre_rating[i] += floor(difference * adjustment_factor);

    current_artists[artist_position].favourites ++;
  }
}

Music * nextSong(const vector <User> & current_users, int user_position, vector <Music> & current_songs)
{
  int difference = 0, total_difference = 0, minimum_val, minimum_position;
  vector <int> overall_differences;
  Music * next_song;

  for(int i = 0; i < sizeof(current_songs); i++)
  {
    for(int j = 0; j < sizeof(current_users[0].profile); j++)
    {
      current_songs[i].associated_genre.genre_rating[j];
    }

    overall_differences.push_back(difference);
    difference = 0;
  }

  minimum_val = overall_differences[0];
  minimum_position = 0;

  for(int t = 0; t < sizeof(overall_differences); t++)
  {
    if(overall_differences[t] < minimum_val)
    {
      minimum_val = overall_differences[t];
      minimum_position = t;
    }
  }

  next_song = current_songs[minimum_position];
  return next_song;
}

void top_level_step (const vector <User> & current_users, int user_position, const vector <Music> & current_songs, int song_position, const vector <Artist> & current_artists, int artist_position)
{
  int type_of_object;
  string name_of_object;
  genre * genre_of_object;
  string name_of_parent_object;
  bool flag = fail;

  cout << "Top level functionallity for creation of new object (song/user/artist)" << endl;
  cout << "Provide input (1 - Song, 2- User, 3 - Artist) about what object you are creating" << endl;

  cin >> type_of_object;

while(!flag)
{
  if(type_of_object == 1)
  {
    cout << "Provide name of Song" << endl;
    cin >> name_of_object;

    cout << "Provide the artist who created the song" << endl;
    cin >> name_of_parent_object;

    genre_of_object = enterGenre();
    entreMusic(name_of_object, name_of_parent_object, genre_of_object, current_artists, current_songs);

    flag = true;
  }

  else if(type_of_object == 2)
  {
    cout << "Provide name of User" << endl;
    cin >> name_of_object;

    genre_of_object = enterGenre();
    enterUser(name_of_object, genre_of_object, vector <User> current_users)

    flag = true;
  }

  else if(type_of_object == 3)
  {
    cout << "Provide name of Artist" << endl;
    cin >> name_of_object;

    genre_of_object = enterGenre();
    enterArtist(name_of_object, genre_of_object, vector <Artist> current_artist);

    flag = true;
  }

  else
  {
    cout << "The value you have entered is invalid!";
  }
}
}
