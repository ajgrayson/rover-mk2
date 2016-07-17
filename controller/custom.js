var ipAddress = "192.168.4.1";

function sendCommand(cmd) {
  $.ajax({
    url: "http://" + ipAddress+ "/action?cmd=" + cmd,
    success: function() {
      enable();
    },
    error: function() {
      enable();
    }
  });
}

function enable() {

}

function disable() {

}
//
// function left() {
//   var val = $("#cmd-left").val();
//   sendCommand("L" + val);
// }
//
// function right() {
//   var val = $("#cmd-right").val();
//   sendCommand("R" + val);
// }

function left() {
    sendCommand('ML-150|MR150');
}
function right() {
    sendCommand('ML150|MR-150');
}
function forward() {
    sendCommand('ML150|vR150');
}
function backward() {
    sendCommand('ML-150|MR-150');
}

function update() {
  var rt = $("#cmd-right").val();
  var lt = $("#cmd-left").val();

  sendCommand("ML" + lt + "|MR" + rt);
}

function sendTextCommand() {
  sendCommand($("#text-command").val());
  $("#text-command").val('');
  $("#text-command").focus();
}

window.onload = function() {
  $('#stop').click(function() {
    sendCommand("STOP");
  });

  $("#dir-l").click(left);
  $("#dir-r").click(right);
  $("#dir-f").click(forward);
  $("#dir-b").click(backward);

  $("#text-command").keyup(function(e) {
    var code = e.which;
    if(code==13){
      e.preventDefault();
      sendTextCommand();
    }
  });

  var timeout = 500;
  var lt;
  var rt;

  $("#cmd-left").bind('keyup mouseup', function() {
    if(lt) {
      clearTimeout(lt);
    }
    lt = setTimeout(update, timeout);
  });

  $("#cmd-right").bind('keyup mouseup', function() {
    if(rt) {
      clearTimeout(rt);
    }
    rt = setTimeout(update, timeout);
  });

}
