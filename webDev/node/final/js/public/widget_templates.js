define(['widget_login','widget_realtimeMediaPresenter','widget_contactList'],function(widget_login,widget_realtimeMediaPresenter,widget_contactList){
console.log("creating widgetbuilder")
var WidgetTemplates ={
	login:widget_login,	
	contactList:widget_contactList,	
	raphaelWindow:widget_realtimeMediaPresenter,
	
	counter: 0,
	create : function(type, socket){
		console.log("Counter:",this.counter, "type:",type, "deps:",socket)
		this.counter = this.counter + 1
		return new this[type](socket)
	},
	register : function(widget, id){

	},
}
return WidgetTemplates
})

