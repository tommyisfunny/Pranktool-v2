var payloadsContainer;
var confirmDeleteDialog;
var confirmDeleteText;
var deleteButton;
var cancelDeleteButton;
var deleteName;

export async function handlePayloads(){
    payloadsContainer = document.getElementById("payloads");
    confirmDeleteDialog = document.getElementById("confirmDeleteDialog");
    confirmDeleteText = document.getElementById("confirmDeleteText");
    deleteButton = document.getElementById("delete");
    cancelDeleteButton = document.getElementById("cancelDelete");

    let response = await fetch("/getPayloads");
    var availablePayloads = await response.json();

    createPayloadList(availablePayloads);
    setupConfirmDialog();
}


function createPayloadList(data){
    var payloads = Object.keys(data);

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