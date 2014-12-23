<?php


namespace Mongo;
class Orchestration {
    protected $baseuri;
    protected $conf;

    function __construct($baseuri) {
        $this->baseuri = $baseuri;
        $this->conf = array(
            "timeout" => 60,
        );
    }

    function ping() {
        try {
            $data = $this->get("");
        } catch(\Exception $e) {
            return false;
        }
        return true;
    }



    function stopAll() {
        $servers = $this->get("servers");
        foreach($servers["servers"] as $server) {
            $this->stopId($server["id"]);
        }
        $servers = $this->get("replica_sets");
        foreach($servers["replica_sets"] as $server) {
            $this->stopReplicaSet($server["id"]);
        }
    }



    function startRS($preset) {
        $relative = __DIR__ . "/../../";
        $file = "scripts/presets/$preset";
        if (!file_exists($relative.$file)) {
            throw new \Exception("Cannot file $file in $relative");
        }

        $retval = $this->post("replica_sets", ["preset" => "/phongo/$file"]);
        return $this->_returnURIIfAlive($retval);
    }
    function start($preset) {
        $relative = __DIR__ . "/../../";
        $file = "scripts/presets/$preset";
        if (!file_exists($relative.$file)) {
            throw new \Exception("Cannot file $file in $relative");
        }

        $retval = $this->post("servers", ["preset" => "/phongo/$file"]);
        return $this->_returnURIIfAlive($retval);
    }

    function getURI($preset) {
        $relative = __DIR__ . "/../../";
        $file = "scripts/presets/$preset";
        $content = file_get_contents($relative.$file);
        $id = json_decode($content, true)["id"];
        return $this->_returnURIIfOK($id);
    }


    function stopReplicaSet($id) {
        try {
            $retval = $this->delete("replica_sets/$id");
            return true;
        } catch(\Exception $e) {
            if ($e->getCode() == 204) {
                return true;
            }

            return false;
        }
    }
    function stopId($id) {
        try {
            $retval = $this->delete("servers/$id");
            return true;
        } catch(\Exception $e) {
            if ($e->getCode() == 204) {
                return true;
            }

            return false;
        }
    }

    function getTimeout() {
        return $this->conf["timeout"];
    }



    function delete($target) {
        $opts = [
            "http" => [
                "timeout" => $this->getTimeout(),
                "method"  => "DELETE",
                "header"  => "Accept: application/json\r\n" .
                             "Content-type: application/x-www-form-urlencoded",
                "ignore_errors" => true,
            ],
        ];

        $data = $this->_sendAndReceive($target, $opts);
        return $data;
    }

    function get($target) {
        $opts = [
            "http" => [
                "timeout" => $this->getTimeout(),
                "method"  => "GET",
                "header"  => "Accept: application/json\r\n" .
                             "Content-type: application/x-www-form-urlencoded",
                "ignore_errors" => true,
            ],
        ];

        $data = $this->_sendAndReceive($target, $opts);
        return $data;
    }

    function post($target, $array) {
        $postdata = json_encode($array);

        $opts = [
            "http" => [
                "timeout" => $this->getTimeout(),
                "method"  => "POST",
                "header"  => "Accept: application/json\r\n" .
                             "Content-type: application/x-www-form-urlencoded",
                "content" => $postdata,
                "ignore_errors" => true,
            ],
        ];

        return $this->_sendAndReceive($target, $opts);
    }



    protected function _returnURIIfOK($id) {
        try {
            $data = $this->get("servers/$id");
            return $data["mongodb_uri"];
        } catch(\Exception $e) {
        }
        try {
            $data = $this->get("replica_sets/$id");
            return $data["mongodb_uri"];
        } catch(\Exception $e) {
            return false;
        }
    }

    protected function _returnURIIfAlive($info) {
        if (!empty($info["mongodb_uri"])) {
            return $info["mongodb_uri"];
        }
        return false;
    }

    protected function _sendAndReceive($target, $opts) {
        $context = stream_context_create($opts);
        $url = $this->baseuri . "/" . $target;

        $http = $opts["http"];

        $hdr = "";
        foreach(explode("\r\n", $http["header"]) as $header) {
            $hdr .= "--header='{$header}' ";
        }

        $debug = sprintf("wget --body-data='%s' --method='%s' %s %s\n",
            isset($http["content"]) ? $http["content"] : "",
            $http["method"],
            $hdr,
            $url
        );
        if (defined("DUMP_WGET") && DUMP_WGET) {
            echo $debug;
        }

        $data = @file_get_contents($url, false, $context);
        if ($data) {
            return json_decode($data, true);
        }
        if (!empty($http_response_header)) {
            sscanf($http_response_header[0], "HTTP/%f %i %s", $proto, $code, $desc);
            throw new \RuntimeException(json_encode($http_response_header), $code);
        }

        throw new \RuntimeException(error_get_last()["message"]);

    }
}

