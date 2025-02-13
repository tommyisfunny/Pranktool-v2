var payloadsContainer;
var confirmDeleteDialog;
var confirmDeleteText;
var deleteButton;
var cancelDeleteButton;
var deleteName;
var clonePayloadDialog;
var payloadFiles;
var payloads;
var payloadToBeCloned;

export async function handlePayloads(){
    payloadsContainer = document.getElementById("payloads");
    confirmDeleteDialog = document.getElementById("confirmDeleteDialog");
    confirmDeleteText = document.getElementById("confirmDeleteText");
    deleteButton = document.getElementById("delete");
    cancelDeleteButton = document.getElementById("cancelDelete");
    clonePayloadDialog = document.getElementById("clonePayloadDialog");
    

    let response = await fetch("/getPayloads");
    payloadFiles = await response.json();
    payloads = Object.keys(payloadFiles);

    createPayloadList();
    setupConfirmDialog();
    setupCloneDialog();
}

function setupCloneDialog(){
    var cancelClonePayload = document.getElementById("cancelClone");
    cancelClonePayload.onclick = function (){
        clonePayloadDialog.close();
    }

    var clonePayloadButton = document.getElementById("clone");
    clonePayloadButton.addEventListener("click", async ()=> {
        var payloadName = document.getElementById("clonedPayloadName").value;

        if(payloadName.length == 0){
            alert("Please enter a name");
            return;
        }

        await fetch("/createPayload?name=" + payloadName);

        copyPayloadFiles(payloadToBeCloned, payloadName);

        clonePayloadDialog.close();
        setInterval(() => {
            location.reload();
        }, 2000);
    });
}

function createPayloadList(){
    var table = document.createElement("table");
    payloads.forEach((payload)=>{
        var tr = document.createElement("tr");
        var th = document.createElement("th");
        th.innerHTML = payload;
        tr.appendChild(th);

        th = document.createElement("th");
        th.appendChild(createEditButton(payload));
        tr.appendChild(th)

        th = document.createElement("th");
        th.appendChild(createRunButton(payload));
        tr.appendChild(th);

        th = document.createElement("th");
        th.appendChild(createCloneButton(payload));
        tr.appendChild(th);

        th = document.createElement("th");
        th.appendChild(createDeleteButton(payload));
        tr.appendChild(th);

        table.appendChild(tr);
    });

    payloadsContainer.appendChild(table);
}

function createEditButton(payload){
    var button = document.createElement("button");
    button.innerHTML = "Edit";
    button.onclick = function(){
        location.href = "/edit.html?payload=" + payload;
    }
    return button;
}

function createRunButton(payload){
    var button = document.createElement("button");
    button.innerHTML = "Run";
    button.onclick = function(){
        fetch("/runPayload?payload=" + payload);
    }
    return button;
};

function createCloneButton(payload){
    var button = document.createElement("button");
    button.innerHTML = "Clone";
    button.onclick = ()=>{
        payloadToBeCloned = payload;
        clonePayloadDialog.showModal();
    }
    return button;
}

function copyPayloadFiles(from, to){
    var filelist = payloadFiles[from];
        filelist.forEach(async (file)=>{
            console.log("get file " + file);
            var response = await fetch("/getFile?file=" + from + "/" + file);
            var filecontent = await response.text();
            console.log("content: " + filecontent);

            if(file == from + ".dd") file = to + ".dd";

            var blob = new Blob([filecontent], {type: 'text/plain'});
            var data = new FormData();
            data.append('file', blob, file);
        
            var resp = await fetch("/uploadFile?path=" + to + "/", {
                method: "POST",
                body: data
            });
            console.log(resp);
        });
}

function createDeleteButton(payload){
    var button = document.createElement("button");
    button.innerHTML = "Delete";
    button.onclick = function(){
        deleteName = payload;
        confirmDeleteText.innerHTML = "<p>Remove <b>" + payload + "</b> from the Device?</p>";
        confirmDeleteDialog.showModal();
    }
    return button;
};

function setupConfirmDialog(){
    deleteButton.onclick = ()=>{
        confirmDeleteDialog.close();
        fetch("/deleteFile?file=" + deleteName).then(()=>location.reload());
    }
    cancelDeleteButton.onclick = ()=>{
        confirmDeleteDialog.close();
    }
}