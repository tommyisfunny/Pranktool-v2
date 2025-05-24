import { handleSettings } from "./settings.js";
import { handlePayloads } from "./payloads.js";
import { handleVersions } from "./versions.js";

var uploadPayloadInput;

window.onload = async function (){
    var updateButton = document.getElementById("update");

    handleSettings();
    handlePayloads();
    handleVersions();

    var createPayloadDialog = document.getElementById("createPayloadDialog");

    document.getElementById("createPayload").onclick = function (){
        createPayloadDialog.showModal();
    }

    var cancelCreatePayload = document.getElementById("cancelCreate");
    cancelCreatePayload.onclick = function (){
        createPayloadDialog.close();
    }

    var createPayloadButton = document.getElementById("create");
    createPayloadButton.addEventListener("click", ()=> {
        var payloadName = document.getElementById("newPayloadName").value;

        if(payloadName.length == 0){
            alert("Please enter a name");
            return;
        }

        fetch("/createPayload?name=" + payloadName).then(()=>{
            location.href = "/edit.html?payload=" + payloadName;
        });
    });

    updateButton.onclick = function (){
        location.href = "/update";
    }

    uploadPayloadInput = document.getElementById("payloadUploadSelector");

    setupFileUpload();
}

function setupFileUpload(){
    uploadPayloadInput.addEventListener("change", async ()=>{
        var file = uploadPayloadInput.files[0];
        var data = new FormData();
        data.append('file', file);

        var name = file.name;
        if(!name.endsWith(".zip")){
            alert("Please upload a .zip file");
            return;
        }

        name = name.substring(0, name.length - 4); // remove .zip extension
        if(name.length == 0){
            alert("Please enter a name");
            return;
        }

        console.log("Uploading payload: " + name);
        console.log("File size: " + file.size + " bytes");
        console.log(data);

        if(!confirm("Are you sure you want to upload a new payload named '" + name + "'? This will overwrite any existing payload with the same name.")){
            return;
        }
        
        // change this 
        var response = await fetch("/uploadZippedPayload?path=" + name, {
            method: "POST",
            body: data
        });
        
        console.log(response);
        if(response.ok){
            location.reload();
        } else {
            alert("Failed to upload payload. Please try again.");
        }
          
    });
}