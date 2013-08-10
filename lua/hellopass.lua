io.write("\027[s") -- save cursor position
l=io.read()
io.write('\027[u',('*'):rep(#l),"\n") -- rewind to where we were, and fill with the correct amount of stars
print("pst, I got", l, "but don't tell anyone!")
