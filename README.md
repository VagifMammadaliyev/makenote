# Note making utility

I mostly work in bash, and sometimes I want to take some notes during my night coding. And I'm always too lazy to open some note taking app with 'awesome' GUI and 1 hour playing animation every time after I type in some symbol. So I decided to make some tool for making notes inside bash (as you have guessed I don't need to close terminal from now on).

### Here how it works
Although I have used this tool for myself for a while and no problems raised, there might be some bugs, as I haven't bothered testig this tool.

But here is how this tool must work:

```bash
makenote 'It is some text!'
``` 
This will create note `It is some text!` and place it in some temporary (`temp`) notebook. This `temp` notebook is really temporary, because all notes saved here will be cleared 10 hours after first note (`It is some text!`) created. Why? It is supposed to be notebook that keeps notes needed only for one night, tomorrow I won't need that notes (i guess).

If you want to save your notes permanently, then add `-i` flag:
```bash
makenote 'It is some important text!' -i
```
This will mark your note as important, so it is note really temporary note. This note is going to stay in your `temp` notebook forever, if you don't clear it explicitly.

You can group your notes too:
```bash
makenote 'Some note about trees' -b treebook
```
This will create `Some note about trees` and save it to `treebook`. Notes that are important ones or saved in some explicitly named notebook are never cleared automatically. You can mark notes in `treebook` notebook as important too:
```bash
makenote "Don't cut trees!" -b treebook -i
```

Now after you have created notes, you must be able to read them too:
```bash
makenote -r a               # remind All notes in 'temp' notebook
makenote -r f               # remind First note (oldest) note in 'temp' notebook
makenote -r l               # remind Last note (newest) note in 'temp' notebook
makenote -r i               # remind Important notes only in 'temp' notebook
makenote -r a -b treebook   # remind All notes in 'treebook' notebook
                            # -r f, -r l, -r i works same as for 'temp' 
                            # for explicitly named notebooks too
```
To clear your notes:
```bash
makenote -c             # clear 'temp' notebook
makenote -c treebook    # clear 'treebook' notebook
```
`-c` makes `makenote` clear notes from notebook, both important and not-important ones. And also it resets 10 hour timer if we are talking about `temp` notebook.

Note: You are not able to add more than one note or clear more than one notebook at once :(

### Installation
This tool is not portable, if someone ever would read this then you might consider improving it by making it portable. Propably, you have to mess with file system routines for each system.

But actually, you can try:
```bash
make install
```
And check if it works (There are chances only on Linux or Mac, not Windows).
