var versionstable;

export async function handleVersions(){
    versionstable = document.getElementById("versions");
    let response = await fetch("/getFirmwareVersion");
    let firmwareVersionData = await response.json();
    response = await fetch("/getFSVersion");
    let FSVersionData = await response.json();

    console.log(firmwareVersionData);
    console.log(FSVersionData);

    var tr = document.createElement("tr");

    var th = document.createElement("th");
    th.innerHTML = "<i>Firmware Version</i>";
    tr.appendChild(th);
    th = document.createElement("th");
    th.innerHTML = "<b>" + firmwareVersionData.version + " " + firmwareVersionData.date + "</b>";
    tr.appendChild(th);
    
    versionstable.appendChild(tr);
    tr = document.createElement("tr");
    
    th = document.createElement("th");
    th.innerHTML = "<i>File System Version</i>";
    tr.appendChild(th);
    th = document.createElement("th");
    th.innerHTML = "<b>" + FSVersionData.version + " " + FSVersionData.date + "</b>";
    tr.appendChild(th);
    
    versionstable.appendChild(tr);
}