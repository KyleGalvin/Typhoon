window.onload = function() {
	console.log("loaded")
	var client = new clientConnection()
//	console.log(client)
	client.connect("192.168.10.105",80)

	console.log("Creating User")
	var username = 'Kyle Galvin'
	var password = '123'
	var makeUser = {}
	makeUser.command='make_user',
	makeUser.args=[username, new Hashes.SHA256().hex(username+password)]
	
	client.write(makeUser)

	console.log("Logging In")
	var login = {}
	login.command='login'
	login.args=[username, Hashes.SHA256().hex(username+password)]
	
	client.write(login)

	//next: get logged in friends
}

