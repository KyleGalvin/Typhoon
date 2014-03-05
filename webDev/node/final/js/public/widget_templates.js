define(['widget_login','widget_realtimeMediaPresenter','widget_contactList'],function(widget_login,widget_realtimeMediaPresenter,widget_contactList){
console.log("creating widgetbuilder")
var WidgetTemplates ={
	login:widget_login,	
	contactList:widget_contactList,	
	raphaelWindow:widget_realtimeMediaPresenter,
	counter: 0,
	widgetLookupTable = {},
	socket : null,
	create : function(type){
		console.log("Counter:",this.counter, "type:",type, "deps:",this.socket)
		this.counter = this.counter + 1
		var widget = new this[type](this.counter,this.socket)
		this.register(widget,this.counter)
		return widget
	},
	register : function(widget, id){
		console.log("before",this.socket.widgetLookupTable)
		this.socket.widgetLookupTable[id] = widget
		console.log("after",this.socket.widgetLookupTable)
	},
}
return WidgetTemplates
})

