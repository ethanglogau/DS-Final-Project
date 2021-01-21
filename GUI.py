import os
import json
import requests
import PySimpleGUIQt as sg
import pandas as pd

from queue import Queue

#have to get a new spotify token before using this
spotify_token = "BQAnB0Nk10jYJrbyFvTLb8UbBxVClB1x-1iJdhNf_6XbMcsgqYn5P7cw5H03lGLxuWDN-xRMqFLojJL7wmEaKPsCjHjP01lrOk51potvO0VYL6g9RzLQxrhri655zw330CCb6Ne95g4ReRU"
class songRec:

    def search_track(self, song_name):
        query = "https://api.spotify.com/v1/search?q=track%3A{}&type=track&offset=0&limit=20".format(
            song_name
        )
        response = requests.get(
            query,
            headers={
                "Content-Type": "application/json",
                "Authorization": "Bearer {}".format(spotify_token)
            }
        )
        response_json = response.json()
        songs = response_json["tracks"]["items"]

        #uri = songs[0]["uri"]
        return songs


    def search_track_and_artist(self, song_name, artist):
        query = "https://api.spotify.com/v1/search?q=track%3A{}+artist%3A{}&type=track&offset=0&limit=20".format(
            song_name,
            artist
        )
        response = requests.get(
            query,
            headers={
                "Content-Type": "application/json",
                "Authorization": "Bearer {}".format(spotify_token)
            }
        )
        response_json = response.json()
        songs = response_json["tracks"]["items"]

        #uri = songs[0]["uri"]
        return songs

def findID2(_song, _artist):
    returnedSongs = songRec.search_track_and_artist("0f2a3b6474214d87b144419b7ac084cd", _song, _artist)
    return returnedSongs[0]["id"]


def findID(_song):
    returnedSongs = songRec.search_track("0f2a3b6474214d87b144419b7ac084cd", _song);
    if len(returnedSongs) > 1:
        return "input artist"
    else:
        return returnedSongs[0]["id"]



returnedSongs = songRec.search_track("0f2a3b6474214d87b144419b7ac084cd", "Feeling Whitney")
for i in returnedSongs:
    print(i["id"])


layout = [[sg.Image(filename="Logo4.png", pad=((0.0),(50,0)))],
          [sg.Text("Song Suggestion Algorithm", key='-TITLE-', justification='left', background_color='NONE', auto_size_text=True, font=["Gotham Medium", 16])],
          [sg.Text("Add Songs you Enjoy One by One to Receive a Personalized Playlist", key='-SUBTITLE-', justification='center', background_color='NONE', auto_size_text=True)],
          [sg.Button('Like', size=(25,1), visible=False), sg.Button('Play', size=(25,1), visible=False), sg.Button('Next', size=(25,1), visible=False)],
          #[sg.Button('Like', size=(25,1), visible=False)],
          [sg.InputText(default_text="Enter your song here", enable_events=True, do_not_clear=True, justification='center', size=(50,1), key='-SONG-'), sg.InputText(default_text="Enter artist name", enable_events=True, do_not_clear=True, justification='center', size=(50,1), key='-ARTIST-', visible=False)],
          [sg.Text(key='-OUTPUT-', justification='center', background_color='NONE', auto_size_text=True)],
          [sg.Button('Add Song', size=(25,1)), sg.Button('Add Artist', size=(25,1), visible=False), sg.Button('Submit', size=(25,1))],
          [sg.Checkbox("Tree Implementation", default=True, key='-Tree-', enable_events=True, background_color='NONE'), sg.Checkbox("Map Implementation", key='-Map-', enable_events=True, background_color='NONE')]]

window = sg.Window('Playlists to improve? Let’s find your groove.', layout, size=(540,960), icon="Logo.ico", resizable=True, element_justification='center', background_image="backb.png")


dschange = 0
idchange = 0
inputtedsongs = []
runtime = 0
songdata = Queue()
currentsong = ['Title','Artist',0,0,0,0]

output = open("LikedSongs.txt", "w")
output.write(str(idchange) + "\n")
output.close()
output2 = open("MorT.txt", "w")
output2.write(str(dschange) + "\n")
output2.write("Tree\n")
output2.close()
dschange += 1
idchange += 1

def updateSongs():
    if os.path.isfile('suggestions.txt'):
        input = open('suggestions.txt')
        x = input.readline()
        global runtime
        if runtime != float(x):
            runtime = float(x)
            input.close()
            df = pd.read_csv('suggestions.txt', skiprows=1, names=['Title:','Artist:','Valence:','Danceability:','Energy:','Acousticness:'])
            for index, row in df.iterrows():
                title = row[0]
                artist = row[1]
                valence = row[2]
                danceability = row[3]
                energy = row[4]
                acousticness = row[5]
                songdata.put([title,artist,valence,danceability,energy,acousticness])
            window['Like'].update(visible=True)
            window['Play'].update(visible=True)
            window['Next'].update(visible=True)
            global currentsong 
            currentsong = songdata.get()
            window['-TITLE-'].update(currentsong[0])
            window['-SUBTITLE-'].update(currentsong[1])
            songdata.put(currentsong)
    else:
        updateSongs()

while True:
    event, values = window.read()
    if event == sg.WINDOW_CLOSED:
        break
    if event == 'Add Song':
        song = values['-SONG-']
        try:
            songID = findID(song)
            if songID == "input artist":
                window['-SONG-'].update(visible=False)
                window['-ARTIST-'].update(visible=True)
                window['Add Artist'].update(visible=True)
                window['Add Song'].update(visible=False)
                window['-OUTPUT-'].update(visible=False)
            else:
                window['-OUTPUT-'].update('Song Added.')
                window['-SONG-'].update(visible=True, value="Enter another song here")
                window['-ARTIST-'].update(visible=False, value="Enter artist name")
                inputtedsongs.append(songID)    
        except:
            window['-OUTPUT-'].update('Song Not Found.')
    if event == 'Add Artist':
        try:
            song = values['-SONG-']
            artist = values['-ARTIST-']
            songID = findID2(song, artist)
            window['-OUTPUT-'].update('Song Added.')
            window['-SONG-'].update(visible=True, value="Enter another song here")
            window['-ARTIST-'].update(visible=False, value="Enter artist name")
            window['Add Artist'].update(visible=False)
            window['Add Song'].update(visible=True)
            inputtedsongs.append(songID)
        except:
            window['-OUTPUT-'].update(visible=True, value='Song Not Found.')
            window['-SONG-'].update(visible=True, value="Enter another song here")
            window['-ARTIST-'].update(visible=False, value="Enter artist name")
            window['Add Artist'].update(visible=False)
            window['Add Song'].update(visible=True)
    if event == 'Submit':
        output = open("LikedSongs.txt", "w")
        output.write(str(idchange) + "\n")
        idchange += 1
        for x in inputtedsongs:
            output.write(x + "\n")
        output.write("Done")
        output.close()
        updateSongs()

    if event == '-Map-':
        output2 = open("MorT.txt", "w")
        output2.write(str(dschange) + "\n")
        if window['-Map-'].Get() == True:
            window['-Tree-'].update(value=False)
            output2.write("map\n")
        else:
            window['-Tree-'].update(value=True)
            output2.write("tree\n")
        output2.close()
        dschange += 1
    if event == '-Tree-':
        output2 = open("MorT.txt", "w")
        output2.write(str(dschange) + "\n")
        if window['-Tree-'].Get() == True:
            window['-Map-'].update(value=False)
            output2.write("tree\n")
        else:
            window['-Map-'].update(value=True)
            output2.write("map\n")
        output2.close()
    if event == '-Next-':
        currentsong = songdata.get()
        window['-TITLE-'].update(currentsong[0])
        window['-SUBTITLE-'].update(currentsong[1])
        songdata.put(currentsong)
    if event == 'Like':
        songID = findID2(currentsong[0],currentsong[1])
        inputtedsongs.append(songID)

   # window['-OUTPUT-'].update('Finding songs similar to: ' + values['-INPUT-'])
   # after writing the IDs of all of the songs inputted, after submit must then read in from the suggested songs text file and display the suggested song along with the visual representation of the 4 elements
   # suggestions.txt is formatted as a csv file with the first thing being the song name, the second being the artist, then the following 4 being valence, then danceability, then energy, then acousticness
   # the first line of suggestions will be a number which corresponds to the runtime for the map or tree implementation, the number is in milliseconds

window.close()