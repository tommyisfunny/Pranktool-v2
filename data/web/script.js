import { handleSettings } from "./settings.js";

window.onload = async function (){
    var updateButton = document.getElementById("update");

    handleSettings();

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

    let response = await fetch("/getPayloads");
    var availablePayloads = await response.json();

    createPayloadList(availablePayloads);

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
