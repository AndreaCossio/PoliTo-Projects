<?php
  include("functions.php");
  check_logged();

  if (empty($_GET["id"])) {
    include("error.php");
    die();
  }

  $db = db_connect();

  /* Execute purchase */
  if (isset($_POST["frmname"]) && $_POST["frmname"] == "purchase") {
    $query_purchase = $db->query("INSERT INTO purchase VALUES ('$_SESSION[user]', $_GET[id])");
  }

  /* Check if user purchased */
  $query_check_purchase = $db->query("SELECT * FROM purchase WHERE User = '$_SESSION[user]' AND Id = $_GET[id]");
  if ($query_check_purchase->num_rows > 0)
    $purch = true;
  else $purch = false;

  /* Get the experience info */
  $query_info = $db->query("SELECT * FROM experience WHERE Id = $_GET[id]");
  $info = $query_info->fetch_assoc();

  if (!$purch && $info['User'] == $_SESSION["user"])
    $purch = true;

  /* Execute vote */
  if (isset($_POST["frmname"]) && $_POST["frmname"] == "vote")
    $query_vote = $db->query("INSERT INTO vote VALUES ('$_SESSION[user]', $_GET[id])");

  /* Check if user can vote */
  $query_check_vote = $db->query("SELECT * FROM vote WHERE User = '$_SESSION[user]' AND Id = $_GET[id]");
  if ($query_check_vote->num_rows == 0 && $info["User"] != $_SESSION["user"] && $purch)
    $can_vote = true;
  else $can_vote = false;

  /* Check votes number */
  $info["Votes"] = count_votes($db, $_GET["id"]);

  /* Check purchases number */
  $info["Purchases"] = count_purchases($db, $_GET["id"]);

  $mail = $db->query("SELECT Mail FROM user WHERE User = '$info[User]'")->fetch_array()[0];

  $db->close();
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <title>EIA TakeIt</title>

  <link href='http://fonts.googleapis.com/css?family=Open+Sans:400,800italic,800,700italic,700,600italic,400italic,600,300italic,300|Oswald:400,300,700' rel='stylesheet' type='text/css'>
  <!-- Bootstrap -->
  <link href="../css/bootstrap.min.css" rel="stylesheet">
  <link href="../css/font-awesome.min.css" rel="stylesheet">
  <link href="../css/owl.theme.css" rel="stylesheet">
  <link href="../css/owl.carousel.css" rel="stylesheet">
  <link href="../css/owl.transitions.css" rel="stylesheet">
  <link href="../css/style.css" rel="stylesheet">
  <link href="../css/own.css" rel="stylesheet">
</head>
<body>
  <!-- Navbar -->
  <header class="st-navbar">
    <nav class="navbar navbar-default navbar-fixed-top clearfix" role="navigation">
      <div class="container"><!-- Brand and toggle get grouped for better mobile display -->
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#sept-main-nav">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="../"><img src="../photos/logo_white.png" alt="" class="img-responsive"></a>
        </div>

        <!-- Collect the nav links, forms, and other content for toggling -->
        <div class="collapse navbar-collapse main-nav" id="sept-main-nav">
          <ul class="nav navbar-nav navbar-right">
            <li><a href="./index.php">Dashboard</a></li>
            <li class="active"><a href="./search.php">Search</a></li>
            <li><a href="./add.php">Add</a></li>
            <li><a href="./profile.php">Profile</a></li>
            <li><a href="../">Get out</a></li>
          </ul>
        </div><!-- /.navbar-collapse -->
      </div>
    </nav>
  </header>

  <section class="home" id="home" data-stellar-background-ratio="0.4">
    <div class="container">
      <div class="row">
        <div class="col-md-12">
            <div class="hero-txt">
              <h2 class="hero-title"><?php echo $info["Title"]; ?></h2>
            </div>
        </div>
      </div>
      <div class="row row-info-exp">
        <div class="col-md-3">
          <i class="fa fa-user"></i> <?php echo $info["User"]; ?>
        </div>
        <div class="col-md-3">
          <i class="fa fa-building"></i> <?php echo $info["Company"]; ?>
        </div>
        <div class="col-md-3">
          <i class="fa fa-briefcase"></i> <?php echo $info["Position"]; ?>
        </div>
        <div class="col-md-3">
          <i class="fa fa-thumbs-up"></i> <?php echo $info["Votes"]; ?>
        </div>
      </div>
    </div>
  </section>

  <div class="container">
    <?php
      if (isset($query_purchase) && $query_purchase == true)
        draw_msg_ok("You successfully purchased this experience!");
      else if (isset($query_purchase) && $query_purchase == false)
        draw_msg_err("There was an error in purchasing this experience!");
    ?>
  </div>

  <!-- Content -->
  <section class="content">
    <div class="container">
      <?php
        /* Purchase button */
        if ($purch) {
          echo "<p>" . $info["Description"] . "</p>";
          echo "<p class=\"center\">
            <a href=\"mailto:$mail\">
            <button type=\"button\" class=\"btn btn-primary\">
              <span class=\"glyphicon glyphicon-envelope\" aria-hidden=\"true\"></span> Get in touch with this person
            </button></a></p>";
        } else {
          echo "<form method=\"post\">
          <p class=\"center\">You haven't purchased this experience yet.</p>
          <p class=\"center\"><button type=\"submit\" class=\"btn btn-success\">
            <span style=\"font-size: 100%;\" class=\"glyphicon glyphicon-euro\"></span> Purchase now!
          </button></p>
          <input type=\"hidden\" name=\"frmname\" value=\"purchase\"/>
          </form>";
        }

        /* Vote button */
        if ($can_vote) {
          echo "<form method=\"post\">
          <p class=\"center\"><button type=\"submit\" class=\"btn btn-success\">
            <span style=\"font-size: 100%;\" class=\"glyphicon glyphicon-thumbs-up\"></span> Vote this experience!
          </button></p>
          <input type=\"hidden\" name=\"frmname\" value=\"vote\"/>
          </form>";
        }
      ?>

      <p class="center"><a href="search.php"><button class="btn btn-primary">
        <span style="font-size: 100%" class="glyphicon glyphicon-arrow-left"></span> Go back
      </button></a></p>
    </div>
  </section>

  <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
  <script src="../js/jquery.min.js"></script>
  <!-- Include all compiled plugins (below), or include individual files as needed -->
  <script src="../js/bootstrap.min.js"></script>
  <script src="../js/jquery.easing.min.js"></script>
  <script src="../js/jquery.stellar.js"></script>
  <script src="../js/jquery.appear.js"></script>
  <script src="../js/jquery.nicescroll.min.js"></script>
  <script src="../js/jquery.countTo.js"></script>
  <script src="../js/jquery.shuffle.modernizr.js"></script>
  <script src="../js/jquery.shuffle.js"></script>
  <script src="../js/owl.carousel.js"></script>
  <script src="../js/jquery.ajaxchimp.min.js"></script>
  <script src="../js/script.js"></script>
</body>
</html>
