define(['widget_login','widget_realtimeMediaPresenter','widget_contactList'],function(widget_login,widget_realtimeMediaPresenter,widget_contactList){
console.log("creating widgetbuilder")
var WidgetTemplates ={
	login:widget_login,	
	contactList:widget_contactList,	
	raphaelWindow:widget_realtimeMediaPresenter,
	counter: 0,
	widgetLookupTable : {},
	socket : null,
	create : function(type,deps){
		console.log("Counter:",this.counter, "type:",type, "deps:",this.socket)
		this.counter = this.counter + 1
		if (deps) {
			console.log('deps:',deps)
			var widget = new this[type](this.counter,this.socket,deps)

		}else{
			console.log('nodeps')
			var widget = new this[type](this.counter,this.socket)

		}
		console.log('registering')
		this.register(widget,this.counter)
		return widget
	},
	register : function(widget, id){
		console.log("before",this.widgetLookupTable)
		this.widgetLookupTable[id] = widget
		console.log("after",this.widgetLookupTable)
	},
}
return WidgetTemplates
})

