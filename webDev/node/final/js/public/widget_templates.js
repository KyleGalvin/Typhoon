define(['widget_login','widget_realtimeMediaPresenter','widget_contactList','widget_leftPanel','widget_git','widget_messaging','widget_webRTC','modelHandler'],function(widget_login,widget_realtimeMediaPresenter,widget_contactList,widget_leftPanel,widget_git,widget_messaging,widget_rtc,modelHandler){
console.log("creating widgetbuilder")
var WidgetTemplates ={
	login:widget_login,	
	contactList:widget_contactList,	
	raphaelWindow:widget_realtimeMediaPresenter,
	leftPanel:widget_leftPanel,
	git:widget_git,
	messaging:widget_messaging,
	rtc:widget_rtc,
	counter: 0,
	widgetLookupTable : {},
	socket : null,
	create : function(type,deps){
		console.log("Counter:",this.counter, "type:",type, "deps:",this.socket)
		this.counter = this.counter + 1
		if (deps) {
			console.log('deps:',deps)
			var widget = new this[type](this,new modelHandler(),deps)

		}else{
			console.log('nodeps')
			var widget = new this[type](this,new modelHandler())
			//var widget = new this[type](this,this.counter,this.socket,new modelHandler())

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

