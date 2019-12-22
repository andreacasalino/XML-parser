class Connection{
    constructor(network_canvas, cleaner, activ_tag, activ_attr){
		this.network = null;
		this.network_JSON = null;
		this.actions = [cleaner, activ_tag, activ_attr];
		this.canvas = network_canvas;
		this.last_element_selected = -1;

        this.server_key = '-';
		let handshake_req = new XMLHttpRequest();
		handshake_req.open('GET', 'http://localhost:8080', false);
		handshake_req.setRequestHeader('Accept', this.server_key + '_hello');
		handshake_req.send();
		this.server_key = handshake_req.response;
		console.log("server key: " + this.server_key);
		
		//let dummy_net = "{\"nodes\":[{\"label\":\"<L0>\",\"color\":\"#505250\",\"id\":\"0\"},{\"label\":\"val=50\",\"shape\":\"box\",\"color\":\"#2FA404\",\"id\":\"1\"},{\"label\":\"<L1_1>\",\"color\":\"#505250\",\"id\":\"2\"},{\"label\":\"word=a\",\"shape\":\"box\",\"color\":\"#2FA404\",\"id\":\"3\"},{\"label\":\"word=b\",\"shape\":\"box\",\"color\":\"#2FA404\",\"id\":\"4\"},{\"label\":\"<L2>\",\"color\":\"#505250\",\"id\":\"5\"},{\"label\":\"word=a\",\"shape\":\"box\",\"color\":\"#2FA404\",\"id\":\"6\"},{\"label\":\"word=b\",\"shape\":\"box\",\"color\":\"#2FA404\",\"id\":\"7\"}],\"edges\":[{\"from\":\"1\",\"to\":\"0\",\"color\":\"#2FA404\"},{\"from\":\"3\",\"to\":\"2\",\"color\":\"#2FA404\"},{\"from\":\"4\",\"to\":\"2\",\"color\":\"#2FA404\"},{\"from\":\"2\",\"to\":\"0\",\"color\":\"#505250\",\"arrows\":\"from\"},{\"from\":\"6\",\"to\":\"5\",\"color\":\"#2FA404\"},{\"from\":\"7\",\"to\":\"5\",\"color\":\"#2FA404\"},{\"from\":\"5\",\"to\":\"0\",\"color\":\"#505250\",\"arrows\":\"from\"}]}";
		//this.__Update_network(dummy_net);
	}

	send_command(option, command_ID){
		//console.log(command);		
		var upd_req = new XMLHttpRequest();
		let this_ref = this;
		upd_req.onload = function() { 
			let x =  upd_req.response;
				if(x == 'null'){
					return;
				}
				else {
					this_ref.__Update_network(x);
				}
		}
		
		upd_req.open('GET', 'http://localhost:8080', true);
		let v = this.server_key + '_' + option + "$" + this.last_element_selected + command_ID;
		upd_req.setRequestHeader('Accept', v);
		upd_req.send();
	}

    __Update_network(new_net){
		if (this.network !== null) {
			this.network.destroy();
			this.network = null;
		}
		let options = {
			layout: {
				hierarchical: {
					direction: "UD"
				}
			},
			nodes: {font: {color: '#ffffff'}},
			interaction:{hover:true, selectable: true}	
		};
		this.network_JSON = JSON.parse(new_net);
		this.network = new vis.Network(this.canvas, this.network_JSON, options);
		
		let this_ref = this;
		this.network.on("click", function (params) {
			params.event = "[original event]";
			this_ref.last_element_selected = parseInt(params.nodes[0]);

			this_ref.actions[0]();
			if(this_ref.network_JSON.nodes[this_ref.last_element_selected].hasOwnProperty('shape')){
				this_ref.actions[2]();
			}
			else{
				this_ref.actions[1]();
			}
		});
		
		this.network.on("deselectNode", function (params) {
			this_ref.actions[0]();
			this.last_element_selected = -1;
		});	            
	}     	

}