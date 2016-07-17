var ipAddress = "192.168.4.1";

function dooer() { }
dooer.prototype.run = function() {
  if (this.func){
    this.func();
  }
}
dooer.prototype.dothis = function(fnc, tim) {
  fnc();
  var d = new dooer();
  setTimeout(function() {
    d.run();
  }, tim || 5000);
  return d;
}
dooer.prototype.then = function(fnc, tim) {
  var d = new dooer();
  this.func = function() {
    fnc();
    setTimeout(function() {
      d.run();
    }, tim || 5000);
  }
  return d;
}

function checkConnection() {
  $.ajax({
    url: "http://" + ipAddress + "/action",
    success: function() {
      $('#conn-status').removeClass('disconnected').addClass('connected').text('CONNECTED');
      setTimeout(checkConnection, 30000);
    },
    error: function() {
      $('#conn-status').removeClass('connected').addClass('disconnected').text('NOT CONNECTED');
      setTimeout(checkConnection, 5000);
    }
  })
}

checkConnection();

function createLog(cmd) {
  var en = $('<div class="entry pending">' + cmd + "</div>");
  var child = $('#log').find('.entry').first();
  if(child.length == 0) {
    $('#log').append(en);
  } else {
    child.before(en);
  }
  return en;
}

function updateLog(en, success) {
  if(success) {
    en.removeClass('pending').addClass('success');
  }else{
    en.removeClass('pending').addClass('failed');
  }
}

function sendCommand(cmd) {
  var cmdup = cmd.toUpperCase();
  var logEntry = createLog(cmdup);

  $.ajax({
    url: "http://" + ipAddress+ "/action?cmd=" + cmdup,
    success: function() {
      return function() {
        updateLog(logEntry, true);
        enable();
      }
    }(),
    error: function() {
      return function() {
        updateLog(logEntry, false);
        disable();
      }
    }()
  });
}

function enable() {

}

function disable() {

}

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

function doRoutine(cmd) {
  var d = new dooer();
  d.dothis(forward).then(left, 10000).then(right);
}

function sendTextCommand() {
  var cmd = $("#text-command").val().toUpperCase();
  if(cmd.indexOf('ROUTINE') == 0) {
    doRoutine(cmd);
  } else{
    sendCommand(cmd);
  }
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

  $('#execute-text-command').click(sendTextCommand);

}
