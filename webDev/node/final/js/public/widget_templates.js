define([
		'widget_login',
		'widget_realtimeMediaPresenter',
		'widget_contactList',
		'widget_leftPanel',
		'widget_git',
		'widget_messaging',
		'widget_webRTC',
		'widget_editor',
		'widget_centerPanel',
		'widget_keyshare',
		'modelHandler',
	],
	function(
		widget_login,
		widget_realtimeMediaPresenter,
		widget_contactList,
		widget_leftPanel,
		widget_git,
		widget_messaging,
		widget_rtc,
		widget_editor,
		widget_centerPanel,
		widget_keyshare,
		modelHandler
	){
	console.log("creating widgetbuilder")
	var WidgetTemplates ={
		login:widget_login,	
		contactList:widget_contactList,	
		raphaelWindow:widget_realtimeMediaPresenter,
		leftPanel:widget_leftPanel,
		git:widget_git,
		messaging:widget_messaging,
		rtc:widget_rtc,
		editor:widget_editor,
		centerPanel:widget_centerPanel,
		keyshare:widget_keyshare,
		counter: 0,
		widgetLookupTable : {},
		widgetNameTable : {},
		socket : null,
		create : function(type,deps){
			console.log("Counter:",this.counter, "type:",type)
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
			this.register(widget,this.counter,type)
			return widget
		},
		register : function(widget, id,type){
			console.log("before",this.widgetLookupTable)
			this.widgetLookupTable[id] = widget
			console.log("after",this.widgetLookupTable)
			if(! this.widgetNameTable[type]){
				this.widgetNameTable[type] = []
			}
			this.widgetNameTable[type].push(widget)
		},
	}
	return WidgetTemplates
})

