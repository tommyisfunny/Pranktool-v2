import { handleSettings } from "./settings.js";
import { handlePayloads } from "./payloads.js";

window.onload = async function (){
    var updateButton = document.getElementById("update");

    handleSettings();
    handlePayloads();

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
}

