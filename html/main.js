async function fetchJSON(url) {
    const res = await fetch(url);
    if (!res.ok) throw new Error(`Error - ${url}`);
    return res.json();
}

function formatBytes(bytes) {
    const units = ["B", "KB", "MB", "GB", "TB"];
    let i = 0;
    while (bytes >= 1024 && i < units.length - 1) {
        bytes /= 1024;
        i++;
    }
    return `${bytes.toFixed(1)} ${units[i]}`;
}

function formatUptime(ms) {
    const totalSec = Math.floor(ms / 1000);
    const h = Math.floor(totalSec / 3600);
    const m = Math.floor((totalSec % 3600) / 60);
    const s = totalSec % 60;
    return `${h}h ${m}m ${s}s`;
}

function formatBatteryLife(seconds) {
    // The number of seconds of battery life when at full charge,
    // or –1 if full battery lifetime is unknown or if the device is connected to AC power.
    if (seconds <= 0 || seconds === 0xffffffff) return "Unknown";
    const h = Math.floor(seconds / 3600);
    const m = Math.floor((seconds % 3600) / 60);
    return `${h}h ${m}m`;
}

async function updateDashboard() {
    try {
        const mem = await fetchJSON("/api/meminfo");
        document.getElementById("totalphys").textContent = formatBytes(mem.totalphys);
        document.getElementById("availphys").textContent = formatBytes(mem.availphys);
        document.getElementById("memload").textContent = mem.memoryload;

        const res = await fetchJSON("/api/resolution");
        document.getElementById("scrw").textContent = res.scrw;
        document.getElementById("scrh").textContent = res.scrh;

        const sys = await fetchJSON("/api/system");
        document.getElementById("uptime").textContent = formatUptime(sys.uptime);
        document.getElementById("battery_percentage").textContent = sys.battery_percentage == 0xff ? "Unknown" : `${sys.battery_percentage}%`;
        document.getElementById("battery_duration").textContent = formatBatteryLife(sys.battery_duration);

        const user = await fetchJSON("/api/userinfo");
        document.getElementById("username").textContent = user.username;

        const serialPorts = await fetchJSON("/api/serialports");
        const list = document.getElementById("serial-ports");
        list.innerHTML = "";

        for (port of serialPorts) {
            const child = document.createElement("a");
            child.innerHTML = `<b>${port['DeviceName']}</b> ${port['PortName']}`;
            list.appendChild(child);
        }

    } catch (err) {
        console.error("Update failed - ", err);
    }
}

setInterval(updateDashboard, 2000);
updateDashboard();

const txtbox = document.getElementById("mboxtext");
txtbox.addEventListener("keydown", ev => {
    if (ev.key === "Enter" && ev.target.value.trim() !== "") {
        fetch("/api/messagebox", {
            method: "POST",
            body: ev.target.value
        });
        ev.target.value = "";
    }
});

// Installed app display list
const uninstallList = document.getElementById("uninstall-list");

// Uninstall btn
const uninstallConfirm = document.getElementById("uninstall-confirm");
uninstallConfirm.addEventListener("click", (e) => {
    e.preventDefault();

    fetch("/api/uninstallapp", {
        method: "POST",
        body: uninstallList.value
    })
})

// Refresh btn

const uninstallRefresh = document.getElementById("uninstall-refresh");
async function refreshInstallList() {
    const data = await fetchJSON("/api/installedapps");
    uninstallList.innerHTML = ""; // clear children

    for (entry of data) {
        const child = document.createElement("option");
        child.value = entry['UninstallString'];
        child.innerText = entry['DisplayName'];
        uninstallList.appendChild(child);
    }
}
refreshInstallList();

uninstallRefresh.addEventListener("click", (e) => {
    e.preventDefault();

    refreshInstallList();
})