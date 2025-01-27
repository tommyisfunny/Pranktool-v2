window.onload = async function (){
    var autostart = document.getElementById("autostart");
    var button1 = document.getElementById("button1");
    var button2 = document.getElementById("button2");
    var button3 = document.getElementById("button3");
    var button4 = document.getElementById("button4");
    var standartDelay = document.getElementById("standartDelay");
    var SSID = document.getElementById("SSID");
    var password = document.getElementById("password");
    var hidden = document.getElementById("hidden");
    var wlanonboot = document.getElementById("wlanonboot");
    var ledsenabled = document.getElementById("ledsenabled");
    var updateButton = document.getElementById("update");

    var input = document.getElementById("payloadUploadSelector");
    input.addEventListener("change", ()=>{
        var file = input.files[0];
        var data = new FormData();
        data.append('file', file);
        
        fetch("/uploadPayload", {
            method: "POST",
            body: data
        }).then((response)=>{
                console.log(response) // if the response is a JSON object
                location.reload()
            }
          ).catch(
            error => console.log(error) // Handle the error response object
          );
        
          
    });

    let  response = await fetch("/getSettings");
    const settingsData = await response.json();
    
    response = await fetch("/getPayloads");
    var availablePayloads = await response.json();

    createPayloadList(availablePayloads);

    Object.keys(settingsData).forEach((id)=>{
        createSettingsOptions(id, settingsData, availablePayloads);
    });


    document.getElementById("changeSettings").onclick = function (){
        var url = "changeSettings?";
        url += "autostart=" + autostart.value;
        url += "&&button1=" + button1.value;
        url += "&&button2=" + button2.value;
        url += "&&button3=" + button3.value;
        url += "&&button4=" + button4.value;
        url +=  "&&standartDelay=" + standartDelay.value;
        url += "&&SSID=" + SSID.value;
        url += "&&password=" + password.value;
        url += "&&hidden=" + (hidden.checked ? "true" : "false");
        url += "&&wlanonboot=" + (wlanonboot.checked ? "true" : "false");
        url += "&&ledsenabled=" + (ledsenabled.checked ? "true" : "false");

        if(password.value.length < 8 && password.value.length != 0){
            alert("password to short");
            return;
        }

        fetch(url).then(()=>{
            var savedDialog = document.getElementById("settingsSavedDialog");

            savedDialog.showModal();
            window.setTimeout(()=>{
                savedDialog.close();
            }, 2000);
        });
    }

    var createPayloadDialog = document.getElementById("createPayloadDialog");

    document.getElementById("createPayload").onclick = function (){
        createPayloadDialog.showModal();
    }

    var cancelCreatePayload = document.getElementById("cancelCreate");
    cancelCreatePayload.onclick = function (){
        createPayloadDialog.close();
    }

    var createPayloadButton = document.getElementById("create");
    createPayloadButton.onclick = function (){
        var payloadName = document.getElementById("newPayloadName").value;

        if(payloadName.length == 0){
            alert("Please enter a name");
            return;
        }

        fetch("/createPayload?name=" + payloadName).then(()=>{
            location.href = "/edit.html?payload=" + payloadName;
        });
    }

    updateButton.onclick = function (){
        location.href = "/update";
    }
}



function createPayloadList(data){
    var payloadlist = document.getElementById("payloads");

    console.log(data);

    var content = "<table>";
    for(i = 0; i < data.length; i++){
        content += "<tr>";
        content += "<th>";
        content += data[i]; 
        content += "</th>";
        content += "<th>";
        content += " <button class=\"payloadDeleteButton\" name=\""+ data[i] +"\">Delete</button>";
        content += "</th>";
        content += "<th>";
        content += " <button class=\"payloadRunButton\" name=\""+ data[i] +"\">Run</button>";
        content += "</th>";
        content += "<th>";
        content += " <button class=\"payloadEditButton\" name=\""+ data[i] +"\">Edit</button>";
        content += "</th>";
        content += "</tr>";
    }
    content +="</table>";
    payloadlist.innerHTML = content;

    var deleteButtons = document.getElementsByClassName("payloadDeleteButton");
    console.log(deleteButtons);

    var runButtons = document.getElementsByClassName("payloadRunButton");
    console.log(runButtons);

    var editButtons = document.getElementsByClassName("payloadEditButton");
    console.log(editButtons);

    for(var i = 0; i < runButtons.length; i++){
        runButtons[i].onclick = function (){
            fetch("/runPayload?payload=" + this.name);
        }
    }

    var confirmDeleteDialog = document.getElementById("confirmDeleteDialog");
    var confirmDeleteText = document.getElementById("confirmDeleteText");
    var deleteButton = document.getElementById("delete");

    for(var i = 0; i < deleteButtons.length; i++){
        deleteButtons[i].onclick = function() {
            deleteName = this.name
            confirmDeleteText.innerHTML = "<p>Remove <b>" + deleteName + "</b> from the Device?</p>";
            confirmDeleteDialog.showModal();
        }
    }

    for(var i = 0; i < editButtons.length; i++){
        editButtons[i].onclick = function (){
            location.href = "/edit.html?payload=" + this.name;
        }
    }

    var cancelDeleteButton = document.getElementById("cancelDelete");

    cancelDeleteButton.onclick = ()=>{
        confirmDeleteDialog.close();
    };

    var deleteName = "";

    deleteButton.onclick = () => {
        fetch("/deletePayload?payload=" + deleteName).then(()=>{
            location.reload();
        });
        confirmDeleteDialog.close();
    }
}

function createSettingsOptions(id, settingsData, availablePayloads){
    var element = document.getElementById(id);

    console.log("getting element: " + id);

    if(element.nodeName == "INPUT"){
        if(element.type == "checkbox"){
            console.log("checkbox set to: " + settingsData[id]);
            element.checked = settingsData[id];
        } else {
            console.log("input set to: " + settingsData[id]);
            element.value = settingsData[id];
        }
    } else {
        availablePayloads.forEach((payload) => {
            var option = document.createElement("option");
            option.innerText = payload;
            option.value = payload;
            if(payload == settingsData[id]){
                option.selected = true;
            }
            element.appendChild(option);
        });
    }
}